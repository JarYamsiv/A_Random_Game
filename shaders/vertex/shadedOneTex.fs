#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 txCord;
layout (location = 3) in vec3 aOffset;

out vec3 Normal;
out vec3 FragPos;
out vec2 textureCord;

uniform mat4 model;
uniform mat4 PVM;

void main()
{
	Normal=aNormal;
	textureCord=txCord;
	gl_Position =PVM*vec4(aPos+aOffset, 1.0);
	FragPos=vec3(model * vec4(aPos+aOffset, 1.0));
    //gl_Position=model*vec4(aPos,1.0);
}