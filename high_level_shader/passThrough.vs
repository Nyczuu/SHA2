#version 430 core
layout(location = 0) in vec4 vPosition;
uniform mat4 MV;
out vec4 colorV;
void main()
{
  colorV =  MV * 1.5 * vPosition;
  gl_Position = colorV;
}
