#include "mygame.h"
#include "game.h"

void Entity::setName(std::string n) { name = n; };
void Entity::setModel(Matrix44 m) { model = m; };

EntityMesh::EntityMesh() {
    type = MESH;
    model.setIdentity();
}

EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color){//, Vector3 init_pos){
    this->mesh = mesh;
    this->texture = texture;
    this->shader = shader;
    this->color = color;
    
    type = MESH;
    model.setIdentity();
    //model.setTranslation(init_pos.x, init_pos.y, init_pos.z);
}

void EntityMesh::render()
{
    //get the last camera that was activated
    Camera* camera = Camera::current;
    
    //enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_color", color);
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);
    shader->setUniform("u_time", Game::instance->time);
    shader->setUniform("u_time_coef", t_coef);
    shader->setUniform("u_texture_tiling", tiling);

    //render the mesh using the shader
    mesh->render(GL_TRIANGLES);
    //mesh->renderBounding(model);

    //disable the shader after finishing rendering
    shader->disable();
}

void World::renderWorld(){
    //int cs = game->tileset->width / 16; 
	//PAINTS EACH CELL (BACKGROUND)
    Camera* camera = Game::instance->camera;
    sky->model.setTranslation(camera->eye.x,camera->eye.y,camera->eye.z);
    sky->render();
    
    sea->render();
    int island_index=0;
    EntityMesh * eM;
	for (int x = 0; x < gamemap->width; ++x)
		for (int y = 0; y < gamemap->height; ++y)
		{
			//get cell info
			sCell& cell = gamemap->getCell(x, y);
			if(cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			if (type == ISLAND){
                eM = islands[island_index]->mesh;
                island_index +=1;
			}
			else if (type >= WATER1){
				eM = seapath;
                eM->model.setTranslation(x*tile_offset, 1, y*tile_offset); 
			}
			//compute tile pos in tileset image
			
			//avoid rendering out of screen stuff

			//draw region of tileset inside framebuffer
			eM->render();  								//area  						
		}

    for(int i=0; i<3;i++){
        //all_npc[i]->mesh->model.setTranslation(all_npc[i]->pos.x,all_npc[i]->pos.y,all_npc[i]->pos.z);
        all_npc[i]->mesh->render();
    }
    //render player
    //boat->mesh->model.setTranslation(boat->pos.x,boat->pos.y,boat->pos.z);
    boat->mesh->render();
}

Island::Island(Vector3 pos, eIslandType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
    mesh->model.setTranslation(pos.x,pos.y,pos.z);
    mesh->model.scale(0.15, 0.15, 0.15);
    
    npc_vec = Vector3(0, 0, 0);
    for (int l=0; l<8; l++){links[l] = false;}
    //UNA POSSIBILITAT SERIA GUARDAR AQUÍ UN INT (QUE DE FET JA ES EL ENUM)
    //FENT REFERENCIA A UNA LLISTA DE Entities O MESHES DE ISLANDS (DO design jeje)
    //Una prova de com podria anar està a main
};

NPC::NPC(Vector3 pos, eNPCType type, EntityMesh* mesh){
    this->pos = pos;
    this->type = type;
    this->mesh = mesh;
    mesh->model.setTranslation(pos.x,pos.y,pos.z);
}

Player::Player(Vector3 init_pos, Island* current_island, EntityMesh* mesh){
    this->pos = init_pos;
    this->current_island = current_island;
    this->mesh = mesh;
    this->current_NPC = NULL;
    
    //this->mesh->model.translate(init_pos.x, init_pos.y, init_pos.z);
    Game::instance->world->boat = this;
}

int World::moveTo(Island* dest){
    Island* orig = boat->current_island;
    if (orig == NULL) return -1;
    else if (orig == dest) return -1;
    else if (boat->movesAlone==1) {
        std::cout << "You already went solo once" << std::endl;
        return -1;
    }
    int ok = leave(orig);
    if (ok==0){
        std::cout << "Left from: " << orig->index_inVector << std::endl;
        ok = arrive(dest);
        if (ok) std::cout << "Problem arriving to: " << dest->index_inVector << std::endl;
        else std::cout << "Arrived at: " << dest->index_inVector <<std::endl;
    }
    else std::cout << "Problem leaving from: " << orig->index_inVector <<std::endl;
    if (boat->current_NPC==NULL) boat->movesAlone += 1;
    //else boat->movesAlone = 0;
    return ok;
}

int World::leave(Island* island){
    if (island->npc_vec[0] && island->npc_vec[1]) return 1; //WOLF EATS SHEEP
    else if (island->npc_vec[1] && island->npc_vec[2]) return 1; //SHEEP EATS CABBAGE
    else {
        boat->current_island = NULL;
        return 0;
    }
}

int World::arrive(Island* island){
    boat->current_island = island;
    if (island->type == WOLVES && boat->current_NPC->type==SHEEP){return 1;}
    else if (island->type == LIONS && (boat->current_NPC->type==WOLF ||
                                       boat->current_NPC->type==SHEEP)){return 1;}
    else if (island->type == PLAGUE && boat->current_NPC->type==CABBAGE){return 1;}
    else if (island->type == SHEPHERD && boat->current_NPC->type==WOLF){return 1;}
    else if (island->type == SHEEP_PARTY && (boat->current_NPC->type==WOLF ||
                                             boat->current_NPC->type==CABBAGE)){return 1;}
    else return 0;
}

void World::drop(){
    NPC* n = boat->current_NPC; //std::cout<<"a"<<std::endl;
    Island* i = boat->current_island; //std::cout<<"b"<<std::endl;
    i->addNPC(n); //std::cout<<"c"<<std::endl;
    //n->mesh->model.setTranslation(new_pos.x,new_pos.y,new_pos.z);
    boat->current_NPC = NULL; //std::cout<<"f"<<std::endl;
}

void World::pickup(NPC* npc){
    Island* i = boat->current_island;
    i->removeNPC(npc);
    Vector3 new_pos = 
        Vector3(boat->pos.x+10,
                boat->pos.y,
                boat->pos.z);
    npc->pos = new_pos;
    //npc->pmesh->model.setTranslation(new_pos.x,new_pos.y,new_pos.z);
    boat->current_NPC = npc;
    boat->movesAlone = 0;
}

void World::setup_level(TileMap* map){

    Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/illumination.fs"); //TODO

    gamemap = map;
    
    islands.clear();
    int island_index = 0;
    for(int x = 0; x < map->width; x++){
        for(int y = 0; y < map->height; y++){
            if(map->getCell(x,y).type==ISLAND){
                EntityMesh* eMi = new EntityMesh(Game::instance->mesh_island, Game::instance->texture_atlas, shader, Vector4(1-island_index/5.0, 1-island_index/5.0, 1-island_index/5.0, 1-island_index/5.0));
                Island* island = new Island(Vector3(x*tile_offset, 1, y*tile_offset), NORMAL, eMi);
                island->tilemap_pos = Vector2(x,y);
                island->index_inVector = island_index;
                island_index += 1;
                islands.push_back(island);
            }
        }
    }
    int step;
    Vector2 directions[8] = {Vector2(0,-1), Vector2(1,0), Vector2(-1,0), Vector2(0,1),
                             Vector2(1,-1), Vector2(-1,-1), Vector2(1,1), Vector2(-1,1)};
    for (int i = 0; i<islands.size(); i++){
        Island* orig = islands[i];
        for (int d = 0; d<8; d++){
            step = 1;
            while(map->getCell(orig->tilemap_pos+directions[d]*step).type>=WATER1){
                step += 1;
            }
            if (map->getCell(orig->tilemap_pos+directions[d]*step).type == ISLAND){
                orig->links[d] = true;
            }
        }
    }

    all_npc.clear();
    EntityMesh* penguin_m1 = new EntityMesh(Game::instance->mesh_bear, Game::instance->texture_atlas, shader, Vector4(1, 1, 1, 1));
    NPC* penguin_w = new NPC(Vector3(0, 0, 0), WOLF, penguin_m1);
    all_npc.push_back(penguin_w);

    EntityMesh* penguin_m2 = new EntityMesh(Game::instance->mesh_penguin, Game::instance->texture_atlas, shader, Vector4(1, 1, 1, 1));
    NPC* penguin_s = new NPC(Vector3(5, 0, 0), SHEEP, penguin_m2);
    all_npc.push_back(penguin_s);

    EntityMesh* penguin_m3 = new EntityMesh(Game::instance->mesh_rat, Game::instance->texture_atlas, shader, Vector4(1, 1, 1, 1));
    NPC* penguin_c = new NPC(Vector3(10, 0, 0), CABBAGE, penguin_m3);
    all_npc.push_back(penguin_c);

    islands[0]->addNPC(penguin_w); islands[0]->addNPC(penguin_s); islands[0]->addNPC(penguin_c);
   
    EntityMesh* boat_m = new EntityMesh(Game::instance->mesh_boat, Game::instance->texture_atlas, shader, Vector4(1, 1, 1, 1));
    boat = new Player(islands[0]->pos, islands[0], boat_m);
}

TileMap* loadGameMap(const char* filename)
{
    FILE* file = fopen(filename,"rb");
    if (file == NULL) //file not found
        return NULL;
    sMapHeader header; //read header and store it in the struct
    fread( &header, sizeof(sMapHeader), 1, file);
    assert(header.bytes == 1); //always control bad cases!!

    //allocate memory for the cells data and read it
    unsigned char* cells = new unsigned char[ header.w*header.h ];
    fread( cells, header.bytes, header.w*header.h, file);
    fclose(file); //always close open files
    //create the map where we will store it
    TileMap* map = new TileMap(header.w,header.h);

    for(int x = 0; x < map->width; x++)
        for(int y = 0; y < map->height; y++){
            map->getCell(x,y).type = (eCellType)cells[x+y*map->width];
	}
	//std::cout << map->n_spawn << ", " << map->n_field << std::endl;
				
    delete[] cells; //always free any memory allocated!
    
    return map;
}
