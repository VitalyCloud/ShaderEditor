#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Color;

void main()
{
   	color = vec4(v_Color, 1.0f);
   	//color = vec4(0.3f, 0.5f, 0.7f, 1.0f);
}
