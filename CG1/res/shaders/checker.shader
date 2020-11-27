#shader VERTEX triangle
#version 330 core
layout(location = 0) in vec4 position;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * position;
};

#shader FRAGMENT color_light_blue
#version 330 core

void main()
{
	gl_FragColor = vec4(0.53, 0.76, 0.95, 1.0);
};

#shader FRAGMENT color_blue
#version 330 core

void main()
{
	gl_FragColor = vec4(0.32, 0.2, 0.92, 1.0);
};