//fragment shader
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 textureCord;

uniform sampler2D ourTexture;
uniform vec3 lightPos=vec3(0.0,10.0,0.0);
uniform vec3 lightColor=vec3(1.0,1.0,1.0);
uniform vec3 objectColor=vec3(1.0,1.0,1.0);

void main()
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);  
    vec3 lightDir = vec3(0.0,1.0,0.0);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    //FragColor = vec4(result, 1.0);
	FragColor = texture(ourTexture,textureCord)*vec4(result, 1.0);  
}

