#version 330 core
	
//Outputs the colour to the pixels
layout (location = 0) out vec4 FragColor;

//input from vertex shader
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
	FragColor = texColor;
};