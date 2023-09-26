#version 430 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aCol;

out vec4 vColor;

void main()
{
  gl_Position = vec4(aPos);
  vColor = aCol;
}