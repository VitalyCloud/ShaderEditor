#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Color;

uniform vec4 u_color;
uniform vec4 u_color2;

void main()
{
    color = vec4(u_color2);
}
