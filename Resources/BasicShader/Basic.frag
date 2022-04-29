#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Color;

uniform vec4 u_color;
uniform vec4 u_color2;

in vec2 TexCoord;
uniform sampler2D wer;

void main()
{
	color = texture(wer, TexCoord) * u_color;
	//color = vec4(vec2(TexCoord), 1.0f, 1.0f);
   	//color = vec4(v_Color, 1.0f);
}
