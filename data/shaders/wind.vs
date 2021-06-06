attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_uv;
attribute vec4 a_color;

uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_viewprojection;

varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    float speed = 1.0;
    float strength = 0.1;
    float detail = 1.0;
    vec2 direction = vec2(1.0, 0.0);
    float heightOffset = 0.0;
    
    
    v_normal = (u_model * vec4( a_normal, 0.0) ).xyz;
    //calcule the vertex in object space
    v_position = a_vertex;
    //store the color in the varying var to use it from the pixel shader
    v_color = a_color;
    //store the texture coordinates
    v_uv = a_uv;
    
    
    v_world_position = (u_model * vec4( v_position, 1.0) ).xyz;

    float time = u_time * speed + v_world_position.x + v_world_position.z;
    float wind = (sin(time) + cos(time * detail)) * strength * max(0.0, v_position.y - heightOffset);
    vec2 dir = normalize(direction);
    v_world_position.xz += vec2(wind * dir.x, wind * dir.y);
    
    gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
    
    //VERTEX = (INV_CAMERA_MATRIX * worldPos).xyz;
    //NORMAL = (MODELVIEW_MATRIX * vec4(NORMAL, 0.0)).xyz;
}
