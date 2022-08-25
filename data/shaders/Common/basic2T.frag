#version 430

layout(location = 0) out vec4 FragColor;

layout(std430, binding = 0) buffer pixelVec { vec4 pixel[]; };

uniform int width;

const vec3 gamma = vec3(1.0 / 2.1);
const float part = 1.0 / 16.0;

void main()
{
#if 1
    //增加一个滤波器
    uint index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y - 1) * width);
    vec3 color = pixel[index++].xyz;
    color += pixel[index++].xyz * 2.1842;
    color += pixel[index].xyz;

    index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y) * width);
    color += pixel[index++].xyz * 2.1842;
    color += pixel[index++].xyz * 4.7707;
    color += pixel[index].xyz * 2.1842;

    index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y + 1) * width);
    color += pixel[index++].xyz;
    color += pixel[index++].xyz * 2.1842;
    color += pixel[index].xyz;

    FragColor = vec4(pow(color * part, gamma), 1.0);
#else

    uint index = (uint(gl_FragCoord.x) + uint(gl_FragCoord.y) * width);
    vec3 color = pow(pixel[index].xyz, gamma);

    FragColor = vec4(color, 1.0);
#endif
}
