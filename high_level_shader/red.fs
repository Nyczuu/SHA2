#version 430 core
in float green;
out vec4 fColor;
void
main()
{
   //fColor = vec4(1.0, 0.0, 0.0, 1.0);
   fColor = vec4(green, green, green, 1.0);
}