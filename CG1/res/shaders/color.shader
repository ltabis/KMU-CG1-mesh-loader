#shader VERTEX triangle
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 FragColor;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * position;
	FragColor = color;
};

#shader FRAGMENT color
#version 330 core
layout(location = 0) out vec4 color;

in vec3 FragColor;

void main()
{
	color = vec4(FragColor, 1.0);
};