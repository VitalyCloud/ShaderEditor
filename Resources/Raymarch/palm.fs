// Author: Vitaly
// Title:
#version 330 core 

layout(location = 0) out vec4 FragColor;

uniform vec2 u_Resolution;
uniform float u_Time;

uniform vec3 u_color1;

void main() {
    vec2 p = gl_FragCoord.xy/u_Resolution.xy;
    vec2 q = p - vec2(0.34, 0.7);

    vec3 color = mix(vec3(u_color1), vec3(1.0, 0.8, 0.3), sqrt(p.y));

    // Plant
    float r = 0.2 + 0.1*cos(atan(q.y, q.x) * 10.0 + 20.0*q.x + 0.5);
    color *= smoothstep(r, r+0.01, length(q));

    // Wood
    r = 0.015;
    r += 0.002*cos(120.00*q.y);
    r += exp(-40.0*p.y);
    color *= 1.0 - (1.0-smoothstep(r, r+0.002, abs(q.x-0.2*sin(2.0*q.y)))) *(1.0-smoothstep(0.0, 0.1, q.y));


    vec2 q2 = p - vec2(0.8, 0.8);
    r = 0.1;
    color *= smoothstep(r, r+0.01, length(q2));

    FragColor = vec4(color, 1.0);
}

