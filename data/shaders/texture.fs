
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_time_coef;
uniform float u_texture_tiling;

float mod (float a, float b){return a-b*floor(a/b);}
vec2 modv (vec2 a, float b){return vec2(mod(a.x,b),mod(a.y,b));}

/*void main()
{
    float dx = 15.0 * (1.0 / 800.0);
    float dy = 10.0 * (1.0 / 600.0);
    vec2 Coord = vec2(dx * floor(v_uv.x / dx),
                          dy * floor(v_uv.y / dy));
    gl_FragColor = u_color * texture2D(u_texture, Coord);
}*/

void main()
{
	vec2 uv = v_uv;
    float t = u_time*u_time_coef;
	gl_FragColor = u_color * texture2D(u_texture, modv(uv*u_texture_tiling+t,1.0));
} 
