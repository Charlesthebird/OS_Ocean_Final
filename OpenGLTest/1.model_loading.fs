#version 330 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D tex;


void main()
{
	// ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 1; // higher is brighter specular highlight
	float shininess = 32; // higher is shinier (smaller radius of specular highlight)
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  
	


	vec3 result = 4*(ambient + diffuse + specular) * texture(tex, texCoord).xyz;
	FragColor = vec4(result, 1.0);//vec3(.2,.3,.9);
    //FragColor = vec4(result, 1.0);
}  