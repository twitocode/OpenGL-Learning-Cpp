#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

//outputs data from the vertex shader into the fragment shader
//v = varrying
out vec2 v_TexCoord;

void main() {
	gl_Position = position;
	v_TexCoord = texCoord;
};