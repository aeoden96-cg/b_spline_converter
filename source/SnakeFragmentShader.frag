#version 330 core

// interpolirana vrijednost vrijednosti koja je poslana iz vertex shadera
//in vec4 fragmentColor;
in float fragColor;
// Ouput data
out vec4 color;

uniform int  eyes;


void main()
{
	if (fragColor == 300){
		color = vec4(1,1,1,0);
	}
	else if (fragColor == 400){
		color = vec4(0,0,0,0);
	}
	else{
		color = vec4(1-fragColor /20,0,0,0);
	}



}
