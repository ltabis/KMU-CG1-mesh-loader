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

struct LightInfo {
	vec3 AmbiantColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;

	vec4 Position;
	vec3 Intensity;
};

struct Material {
	vec3 AmbiantColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;

	float shininess;
	float opacity;
};

out vec4 FragColor;

uniform Material  u_material;
uniform LightInfo u_light;

uniform mat4 u_view;

uniform sampler2D u_texture_diffuse1;
uniform sampler2D u_texture_specular1;

in vec3 Position;
in vec3 Normal;
in vec2 TxtCoords;

float p = 32.0;

void main()
{
	// to transform into uniforms.
	vec3 diffuseLightColor = vec3(1.0, 1.0, 1.0);
	vec3 specularLightColor = vec3(1.0, 1.0, 1.0);

	vec3 L = normalize(vec3(u_view * u_light.Position) - Position);
	vec3 V = normalize(-Position);
	// computation heavy
	vec3 R = reflect(-L, Normal);

	vec3 ambiantColor = u_material.AmbiantColor * u_light.AmbiantColor;
	vec3 diffuseColor = u_material.DiffuseColor * u_light.DiffuseColor * max(dot(L, Normal), 0.0);
	vec3 specularColor = u_material.SpecularColor * u_light.SpecularColor * pow(max(dot(V, R), 0.0), p);

	vec4 textureColorDiffuse = texture2D(u_texture_diffuse1, TxtCoords);
	vec4 textureColorSpecular = texture2D(u_texture_specular1, TxtCoords);
	vec4 textureColor = mix(textureColorDiffuse, textureColorSpecular, textureColorDiffuse.a);
	FragColor = vec4(ambiantColor + diffuseColor + specularColor, 1.0) * textureColor;
}