#shader VERTEX triangle
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_modelView;
uniform mat3 u_normalMat;

out vec3 Position;
out vec3 Normal;

void main()
{
	Position = vec3(u_modelView * position);
	Normal = normalize(u_normalMat * normal);

	gl_Position = u_mvp * position;
}

#shader FRAGMENT color
#version 330 core
layout(location = 0) out vec4 FragColor;

uniform vec3 u_ambiantLightColor;
uniform vec3 u_objectColor;
uniform vec3 u_lightPos;
uniform mat4 u_view;

in vec3 Position;
in vec3 Normal;

float p = 1.0;

void main()
{
	// to transform into uniforms.
	vec3 diffuseLightColor = vec3(1.0, 1.0, 1.0);
	vec3 specularLightColor = vec3(1.0, 1.0, 1.0);

	vec3 L = normalize(vec3(u_view * vec4(u_lightPos, 1.0)) - Position);
	vec3 V = normalize(-Position);
	vec3 H = V + L;

	vec3 diffuseColor = diffuseLightColor * max(dot(L, Normal), 0.0);
	vec3 specularColor = 0.5 * specularLightColor * pow(max(dot(H, Normal), 0.0), p);

	FragColor = vec4((u_ambiantLightColor + diffuseColor + specularColor) * u_objectColor, 1.0);
};