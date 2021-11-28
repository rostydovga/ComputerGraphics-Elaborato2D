#version 420 core

// Interpolated values from the vertex shaders

in vec4 ourColor;
out vec4 FragColor;  

//uniform int scelta;
uniform float time;
uniform vec2 resolution;
int scelta=1;
#define pi 3.14159235659


void main(){
//Viene assegnato ad ogni frammento il colore interpolato
	//FragColor=ourColor;
	if (scelta==1)
    {
        FragColor = vec4(ourColor);
    }
    
}