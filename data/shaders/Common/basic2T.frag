#version 430
layout(location = 0) out vec4 FragColor;
layout(std430, binding = 0) buffer pixelVec { vec4 pixel[]; };
uniform int width;
const vec3 gamma = vec3(1.0 / 2.1);

void main()
{
#if 0
    //增加一个滤波器
    highp uint index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y - 1) * width);
    vec3 color = pow(pixel[index++].xyz, gamma);
    color += pow(pixel[index++].xyz, gamma) * 2.1842;
    color += pow(pixel[index].xyz, gamma);

    index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y) * width);
    color = pow(pixel[index++].xyz, gamma) * 2.1842;
    color += pow(pixel[index++].xyz, gamma) * 4.7707;
    color += pow(pixel[index].xyz, gamma) * 2.1842;

    index = (uint(gl_FragCoord.x - 1) + uint(gl_FragCoord.y + 1) * width);
    color = pow(pixel[index++].xyz, gamma);
    color += pow(pixel[index++].xyz, gamma) * 2.1842;
    color += pow(pixel[index].xyz, gamma);

    FragColor = vec4(color / 4.7707 , 1.0);
#else
    highp uint index = (uint(gl_FragCoord.x) + uint(gl_FragCoord.y) * width);
    vec3 color = pow(pixel[index].xyz, gamma);

    FragColor = vec4(color, 1.0);
#endif
}
