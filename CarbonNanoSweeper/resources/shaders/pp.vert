#version 430 core

const vec2 corner[4] = {
	vec2(-1, 1),
	vec2(-1,-1),
	vec2( 1, 1),
	vec2( 1,-1)
};

out vec2 v_texcoord;

void main()
{
    gl_Position = vec4(corner[gl_VertexID],0,1);
    v_texcoord = corner[gl_VertexID]*0.5+vec2(0.5);
}
