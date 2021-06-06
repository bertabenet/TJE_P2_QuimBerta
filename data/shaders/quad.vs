attribute vec3 a_vertex;
attribute vec2 a_uv;

varying vec2 v_uv;

void main()
{
	//store the texture coordinates
	v_uv = a_uv;

	//calcule the position of the vertex using the matrices
    gl_Position = vec4( a_vertex, 1.0 );
}
