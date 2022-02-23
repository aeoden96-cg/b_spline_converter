#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
//out vec3 fragmentColor;



void romb(vec4 position)
{

	gl_Position = position + vec4(0.1, 0, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( 0, -0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( -0.1, 0, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0,0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(0.1, 0, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}
void main(){

	romb(gl_in[0].gl_Position);

	gl_Position = gl_in[0].gl_Position + vec4(0.4, 0, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(-0.4, 0, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}

/*
in gl_Vertex
{
	vec4  gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
} gl_in[];
*/