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
   content = (char*) malloc((numVal+1) * sizeof(char)); 
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

   glShaderSource(vertexShaderHandle, 1, (const char**) &vertexShader, NULL); // Sets source code for vertex shader.
   glShaderSource(fragmentShaderHandle, 1, (const char**) &fragmentShader, NULL); // Sets source code for fragment shader.

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
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f(0.0, 0.0, 0.0);

   // Draw square.
   glBegin(GL_POLYGON);
      glVertex3f(20.0, 20.0, 0.0);
      glVertex3f(80.0, 20.0, 0.0);
      glVertex3f(80.0, 80.0, 0.0);
      glVertex3f(20.0, 80.0, 0.0);
   glEnd();

   glFlush();
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(programHandle); // Installs program into current rendering state.
 	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 1)); 
    glUniformMatrix4fv(MV, 1, GL_TRUE, glm::value_ptr(ViewTranslate));
	
	//do saplera
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    
	glBindVertexArray(idVA);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 

   glGenVertexArrays(1, &idVA);
   glBindVertexArray(idVA);
   GLfloat vertices[6][2] = {
		{ -0.90, -0.90 }, // Triangle 1
		{ 0.85, -0.90 },
		{ -0.90, 0.85 },
		{ 0.90, -0.85 }, // Triangle 2
		{ 0.90, 0.90 },
		{ -0.85, 0.90 }
	};
    GLfloat textures[6][2] = {
		{ 0.0, 0.0 }, // Triangle 1
		{ 0.0, 1.0 },
		{ 1.0, 0.0 },
	    { 0.0, 0.0 }, // Triangle 2
		{ 0.0, 1.0 },
		{ 1.0, 0.0 }
	};
    glGenBuffers(1, &idBuf);
    glBindBuffer(GL_ARRAY_BUFFER, idBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(textures), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(textures), textures);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*) sizeof(vertices));
    glEnableVertexAttribArray(1);

	//tekstura
	/*glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, 8, 8);

    static const unsigned char texture_data[] =
    {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    };

    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0, 0,
                    8, 8,
                    GL_RED, GL_UNSIGNED_BYTE,
                    texture_data);



	//utworzenie samplera
	glGenSamplers(1, &samp);
    glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(samp, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glUniform1i(glGetUniformLocation(programHandle, "tex"), 0);
	glBindSampler(0, samp);*/
	
}

// sprawdzenie i przygotowanie obslugi wybranych rozszerzen
void extensionSetup()
{
    // pobranie numeru wersji biblioteki OpenGL
    const char * version =( char * ) glGetString( GL_VERSION );
    
    
    if ((version[0] < '1') || ((version[0] == '1') && (version[2] < '5')) || (version[1] != '.'))  {
        
        printf("Bledny format wersji OpenGL\n");
          
        exit(0);
    }
 	else {
        
       glCreateProgram = ( PFNGLCREATEPROGRAMPROC ) wglGetProcAddress( "glCreateProgram" );
		glCreateShader = ( PFNGLCREATESHADERPROC ) wglGetProcAddress( "glCreateShader" );
        glShaderSource = ( PFNGLSHADERSOURCEPROC ) wglGetProcAddress( "glShaderSource" );
		glAttachShader = ( PFNGLATTACHSHADERPROC ) wglGetProcAddress( "glAttachShader" );
		glLinkProgram = ( PFNGLLINKPROGRAMPROC ) wglGetProcAddress( "glLinkProgram" );
		glUseProgram = ( PFNGLUSEPROGRAMPROC ) wglGetProcAddress( "glUseProgram" );
		glCompileShader = ( PFNGLCOMPILESHADERPROC ) wglGetProcAddress( "glCompileShader" );
		glBindVertexArray = ( PFNGLBINDVERTEXARRAYPROC ) wglGetProcAddress( "glBindVertexArray" );
		glGenVertexArrays = ( PFNGLGENVERTEXARRAYSPROC )  wglGetProcAddress( "glGenVertexArrays" );
		glGenBuffers = ( PFNGLGENBUFFERSPROC ) wglGetProcAddress( "glGenBuffers" );
		glBindBuffer = ( PFNGLBINDBUFFERPROC ) wglGetProcAddress( "glBindBuffer" );
		glVertexAttribPointer = ( PFNGLVERTEXATTRIBPOINTERPROC ) wglGetProcAddress( "glVertexAttribPointer" );
		glBufferData = ( PFNGLBUFFERDATAPROC )  wglGetProcAddress( "glBufferData" );
		glEnableVertexAttribArray = ( PFNGLENABLEVERTEXATTRIBARRAYPROC ) wglGetProcAddress( "glEnableVertexAttribArray" );
		glUniformMatrix4fv = ( PFNGLUNIFORMMATRIX4FVPROC ) wglGetProcAddress( "glUniformMatrix4fv" );
		glGetUniformLocation = ( PFNGLGETUNIFORMLOCATIONPROC )  wglGetProcAddress( "glGetUniformLocation" );
		glSamplerParameterfv = ( PFNGLSAMPLERPARAMETERFVPROC ) wglGetProcAddress( "glSamplerParameterfv" );
		glSamplerParameteri = ( PFNGLSAMPLERPARAMETERIPROC ) wglGetProcAddress( "glSamplerParameteri" );
		glUniform1i = ( PFNGLUNIFORM1IPROC ) wglGetProcAddress( "glUniform1i" );
		glBufferSubData = ( PFNGLBUFFERSUBDATAPROC ) wglGetProcAddress( "glBufferSubData" );
		glTexStorage2D = ( PFNGLTEXSTORAGE2DPROC ) wglGetProcAddress( "glTexStorage2D" );
		glActiveTexture = ( PFNGLACTIVETEXTUREPROC ) wglGetProcAddress( "glActiveTexture" );
		glGenSamplers = ( PFNGLGENSAMPLERSPROC ) wglGetProcAddress( "glGenSamplers" );
		glBindSampler = ( PFNGLBINDSAMPLERPROC ) wglGetProcAddress( "glBindSampler" );
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
   switch(key) 
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
  // setShaders("vsTexture.vs", "fsTexture.fs");
   
   
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);


   

   glutMainLoop();

   return 0;   
}
