#shader VERTEX triangle
#version 430 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat4 u_mvp;
uniform mat4 u_modelView;
uniform mat3 u_normalMat;

out vec3 Position;
out vec3 Normal;
out vec2 TxtCoords;

void main()
{
	Position = vec3(u_modelView * position);
	Normal = normalize(u_normalMat * normal);
	TxtCoords = textureCoords;

	gl_Position = u_mvp * position;
}

#shader FRAGMENT color
#version 430 core

struct Material {
	vec3 AmbiantColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;

	float shininess;
	float opacity;
};

out vec4 FragColor;

uniform Material  u_material;

uniform mat4 u_view;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;

in vec3 Position;
in vec3 Normal;
in vec2 TxtCoords;

float p = 32.0;

void main()
{
	vec3 finalColor = vec3(0.0, 0.0, 0.0);
	vec3 V = normalize(-Position);

	// works if at list one light exists. TODO: fix this.
	vec3 ambiantColor = u_lights[0].Intensity * u_material.AmbiantColor * u_lights[0].AmbiantColor;

	for (int i = 0; i < numberOfLights; i++) {
		vec3 L = normalize(vec3(u_view * u_lights[i].Position) - Position);
		vec3 R = reflect(-L, Normal);
		// vec3 H = V + L;

		vec3 diffuseColor =  u_lights[i].Intensity * u_material.DiffuseColor * u_lights[i].DiffuseColor * max(dot(L, Normal), 0.0);
		vec3 specularColor = u_lights[i].Intensity * u_material.SpecularColor * u_lights[i].SpecularColor * pow(max(dot(V, R), 0.0), p);

		finalColor += ambiantColor + diffuseColor + specularColor;
	}

	vec4 textureColorDiffuse = texture2D(u_texture_diffuse1, TxtCoords);
	vec4 textureColorSpecular = texture2D(u_texture_specular1, TxtCoords);
	vec4 textureColor = mix(textureColorDiffuse, textureColorSpecular, textureColorDiffuse.a);

	FragColor = vec4(finalColor + ambiantColor, 1.0) * textureColor;
}