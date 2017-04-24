#version 150

#define FRONTIER_POS
#define FRONTIER_CLR
#define FRONTIER_NRM

in vec2 FRONTIER_POS in_pos;
in vec4 FRONTIER_CLR in_color;
in vec3 FRONTIER_NRM in_normal;

out vec2 geom_pos;
out vec4 geom_color;
out vec3 geom_normal;

void main()
{
	geom_pos    = in_pos;
	geom_color  = in_color;
	geom_normal = in_normal;
}

