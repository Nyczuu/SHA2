#version 430 core
in vec4 colorV;
out vec4 outputF;
void
main()
{
   //fColor = vec4(1.0, 0.0, 0.0, 1.0);
   outputF= colorV;
}