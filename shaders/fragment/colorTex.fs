
#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 textureCord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture,textureCord)*vec4(vertexColor,0.5);  
}

