#version 430 core

uniform vec3 color;

in vec4 vColor;

out vec4 FragColor;

void main()
{             
    if(vColor == vec4(0.0))
        FragColor = vec4(color, 1.0);
    else
        FragColor = vec4(vColor);
}