#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 v_Color;
out vec2 TexCoord;

void main()
{
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0f, 1.0f);
	TexCoord = a_TexCoord;
}
