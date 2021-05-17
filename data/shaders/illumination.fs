
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;


void main()
{
    vec3 N = normalize(v_normal);
    vec3 L = normalize(vec3(0, 1, 1));
    
    vec3 directional = dot(L, N) * texture2D(u_texture, v_uv).xyz * vec3(0.9, 0.9, 0.9);
    vec3 ambient_light = vec3(0.2);
    
    vec4 color = u_color * texture2D(u_texture, v_uv);
    color.xyz *= ambient_light + directional;
    
    gl_FragColor = color;
} 
