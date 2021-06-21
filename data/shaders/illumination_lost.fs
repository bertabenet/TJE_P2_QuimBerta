
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

uniform vec3 u_eye;


void main()
{
    vec3 N = normalize(v_normal);
    vec3 L = normalize(u_eye - v_world_position);
    
    vec3 directional = dot(L, N) * vec3(0.9, 0.7, 0.3);
    vec3 ambient_light = vec3(0.5);
    
    vec4 color = u_color * texture2D(u_texture, v_uv);
    color.xyz *= ambient_light + directional;
    
    gl_FragColor = color;
} 
