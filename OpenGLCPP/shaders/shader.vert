#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

//outputs data from the vertex shader into the fragment shader
//v = varrying
out vec2 v_TexCoord;

//Model-View-Projection matrix
uniform mat4 u_MVP;

void main() {
	//IT HAS TO BE IN THIS ORDER (matrix * position) OR ELSE IT WILL NOT WORK
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};