#version 330 core

out vec4 color;

uniform vec3  col;

void main()
{

	// Output color
	color = vec4(col,1);

}
