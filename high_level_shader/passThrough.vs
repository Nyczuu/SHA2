#version 430 core
layout(location = 0) in vec4 vPosition;
uniform mat4 MV;
out float green;
void
main()
{
  gl_Position = MV * vPosition;
  green = vPosition.y;
}
