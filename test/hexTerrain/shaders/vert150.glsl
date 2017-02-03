#version 150

in vec2 in_pos;
in vec4 in_color;
in vec3 in_normal;

out vec2 geom_pos;
out vec4 geom_color;
out vec3 geom_normal;

void main()
{
	geom_pos    = in_pos;
	geom_color  = in_color;
	geom_normal = in_normal;
}

