#version 430 core
layout(location = 0) in vec4 vPosition;
uniform mat4 MV;
out vec4 colorV;
void main()
{
  gl_Position = MV * vPosition;
  colorV = vPosition;
}
