
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_texture_tiling;

/*void main()
{
    float dx = 15.0 * (1.0 / 800.0);
    float dy = 10.0 * (1.0 / 600.0);
    vec2 Coord = vec2(dx * floor(v_uv.x / dx),
                          dy * floor(v_uv.y / dy));
    gl_FragColor = u_color * texture2D(u_texture, Coord);
}*/
/*void main()
{
	vec2 uv = v_uv;
    float t = u_time*0.1;
    vec2 tiling = uv * u_texture_tiling;
    vec2 movement = vec2((1.0+mod(t,3.0))*tiling.x,tiling.y);
	gl_FragColor = u_color * texture2D(u_texture, movement);
} 
*/
float mod (float a, float b){return a-b*floor(a/b);}

void main()
{
	vec2 uv = v_uv;
	gl_FragColor = u_color * texture2D(u_texture, uv);
} 
