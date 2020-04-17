#version 430 core
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexture;
out vec2 vs_tex_coord;
void main(void) {
  gl_Position = vPosition;
  vs_tex_coord = vTexture;
}
