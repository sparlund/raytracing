#version 330 core
out vec4 FragColor;

in vec2 out_texture_coord;

uniform sampler2D texture_in;

void main()
{
	FragColor = texture(texture_in, out_texture_coord);
}

