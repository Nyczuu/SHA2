#include <cstdlib>
#include <iostream>
#include <fstream>


#define GLUT_DISABLE_ATEXIT_HACK

//#  include "GL/glew.h"
# include "GL/glut.h"
# include "GL/glext.h"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/gtc/type_ptr.hpp"

using namespace std;

// NEW: Globals:
static int programHandle; // Reference to program object.
static int vertexShaderHandle; // Reference to vertex shader object.
static int fragmentShaderHandle; // Reference to fragment shader object.

GLuint idVA;
GLuint idBuf;
GLuint vPosition;
GLuint vTexture;
GLuint MV;
GLuint tex;
GLuint samp;

PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC	glUseProgram = NULL;
PFNGLCOMPILESHADERPROC	glCompileShader = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC  glBufferData = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLGENSAMPLERSPROC glGenSamplers = NULL;
PFNGLBINDSAMPLERPROC glBindSampler = NULL;

// NEW: Function to read a shader file.
char* readShader(char* aShaderFile)
{
	FILE* filePointer;
	fopen_s(&filePointer, aShaderFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

// NEW: Function to initialize shaders.
void setShaders(char* vertexShaderFile, char* fragmentShaderFile)
{
	char* vertexShader = readShader(vertexShaderFile);
	char* fragmentShader = readShader(fragmentShaderFile);

	programHandle = glCreateProgram(); // Creates empty program object and returns reference to it.
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER); // Creates empty vertex shader object and returns reference to it.
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); // Creates empty fragment shader object and returns reference to it.

	glShaderSource(vertexShaderHandle, 1, (const char**)&vertexShader, NULL); // Sets source code for vertex shader.
	glShaderSource(fragmentShaderHandle, 1, (const char**)&fragmentShader, NULL); // Sets source code for fragment shader.

	glCompileShader(vertexShaderHandle); // Compiles vertex shader shource code.
	glCompileShader(fragmentShaderHandle); // Compiles fragment shader source code.

	glAttachShader(programHandle, vertexShaderHandle); // Attaches vertex shader to program.
	glAttachShader(programHandle, fragmentShaderHandle); // Attaches fragment shader to program.

	glLinkProgram(programHandle); // Links the program.


	MV = glGetUniformLocation(programHandle, "MV");
	tex = glGetUniformLocation(programHandle, "tex");

}

// Drawing routine.
void drawScene0(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	while (1) {
		static float radians = 30.0;
		radians = int(radians + 20);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programHandle);
		glm::mat4 ViewTranslate = glm::mat4(1.0f);
		ViewTranslate = glm::scale(ViewTranslate, glm::vec3(0.2, 0.2, 0.2));
		ViewTranslate = glm::rotate(ViewTranslate, glm::radians(radians), glm::vec3(2.1, 1.5, 1.0));

		glUniformMatrix4fv(MV, 1, GL_TRUE, glm::value_ptr(ViewTranslate));

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0, 0.0);

		// Sciana 1
		glVertex3f(0.6f, 1.4f, 0.0f);
		glVertex3f(-2.0f, 1.5f, -2.0f);
		glVertex3f(2.0f, 2.0f, -2.0f);

		// Sciana 2
		glVertex3f(0.6f, 1.4f, 0.0f);
		glVertex3f(-2.0f, 1.5f, -2.0f);
		glVertex3f(0.6f, -2.0f, -2.0f);

		// Sciana 3
		glVertex3f(0.6f, 1.4f, 0.0f);
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f(0.6f, -2.0f, -2.0f);

		// Sciana 4
		glVertex3f(2.0f, 2.0f, -2.0f);
		glVertex3f(-2.0f, 1.5f, -2.0f);
		glVertex3f(0.6f, -2.0f, -2.0f);
		glEnd();

		glFlush();
		Sleep(10);
	}
}

// sprawdzenie i przygotowanie obslugi wybranych rozszerzen
void extensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char * version = (char *)glGetString(GL_VERSION);

	if ((version[0] < '1') || ((version[0] == '1') && (version[2] < '5')) || (version[1] != '.')) {

		printf("Bledny format wersji OpenGL\n");

		exit(0);
	}
	else {

		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)wglGetProcAddress("glSamplerParameterfv");
		glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
		glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)wglGetProcAddress("glTexStorage2D");
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
		glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	}

}
// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shaders");
	extensionSetup();

	setShaders("passThrough.vs", "red.fs");

	glutDisplayFunc(drawScene0);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMainLoop();

	return 0;
}
