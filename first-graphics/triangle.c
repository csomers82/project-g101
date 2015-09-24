#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define DEBUG
#ifdef DEBUG
	#define SHOUT(x) printf("%d", x)
#endif

#define VEC_SHADER_FN "vertexShader.glsl"
#define FRAG_SHADER_FN "fragmentShader.glsl"



struct Vertex {
	float X;
	float Y;
	float Z;
	float R;
	float G;
	float B;
	float A;
};

int VAO;// vertex array object
int VBO;// vertex buffer object


//struct Vertex * triangle;


// ========================================================
/* The load_shader function reads in an entire file as a
 * character array of size 'len'. If the filename pointed
 * to by 'name' is invalid the function returns 'NULL' */
// --------------------------------------------------------
char * load_shader(char * name, int * len) 
{	
	FILE* fp = NULL;
	char* load = NULL;
	int n = 0;
	fp = fopen(name, "rb");
	
	// verify the file ptr
	if(!fp) 
	{
		fprintf(stderr, "\nUnable to recognize file: \"%s\"", name);
		return(NULL);
	}

	// find file end and record length
	fseek(fp, SEEK_SET, SEEK_END);
	n = (int)ftell(fp);
	rewind(fp);//back to beginning
	assert(n >= 0);

	// allocate space for 'shader string' and fill
	load = malloc(sizeof(char) * (n + 1));
	fread(load, sizeof(char), n, fp);

	// add string null character
	load[n] = '\0';

	// management
	*len = n;
	fclose(fp);
	return(load);

}


int exec_shader(char* glsl, GLenum compilertype )
{
	GLint shader_index; // uint (4bytes) 

	// COMPILE GLSL 
	shader_index = glCreateShader(compilertype);// what type of shader is being compiled
	const char * fixedstr = (const char *)(glsl);
	glShaderSource(shader_index, 1, &fixedstr, 0);
		// index, num of str, arr of str, (0 to search null, otherwise a length)
	glCompileShader(shader_index);
	
	// CLEAN UP AND EXCEPTIONS
	free(glsl);
	
	GLint cstatus;// compile status
	glGetShaderiv(shader_index, GL_COMPILE_STATUS, &cstatus);

	if(cstatus == GL_TRUE) {
		// (returns GLint)
		return(shader_index);
	}

	// ERROR REPORTING
	GLint log_length = 0;
	glGetShaderiv(shader_index, GL_INFO_LOG_LENGTH, &log_length);

	char * log_text = malloc(sizeof(char) * (log_length + 1));
	glGetShaderInfoLog(shader_index, log_length, NULL, log_text);
	log_text[log_length + 1] = '\0';

	fprintf(stderr, "%s", log_text);
	glDeleteShader(shader_index);
	free(log_text);	

}

void CheckGLErrors(void)
{
	// Get the openGL error code
	int error = glGetError();

	// If the error code signifies an error has occurred
	if (error != GL_NO_ERROR)
	{
		// Print the error
		printf("Error: %d\n", error);
	}

}


void init_Vertex(struct Vertex * V, int x, int y, float r, float g, float b) 
{	
	// initializes vertex to (x,y) w/ RGBA = {0, 0, 0, 1}
	(*V).Y = y;
	(*V).X = x;
	(*V).Z = 0.0f;

	(*V).R = r;
	(*V).G = g;
	(*V).B = b;
	(*V).A = 1.0f;
}

void Init_Gs(struct Vertex * triangle)
{	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// (PROGRAM SPECIFIC)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Background as opaque blk
		//expects GLfloats (4 byte float type)


	// clear all data from triangle and init coordinate positions
	memset(triangle, 0, (sizeof(struct Vertex) *3) );
	init_Vertex(&(triangle[0]), -1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
	init_Vertex(&(triangle[1]),  1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	init_Vertex(&(triangle[2]),  0.0f,  1.0f, 0.0f, 0.0f, 1.0f);



	// create and index Shader Program
	char * Vglsl = NULL;// Vertex Shader
	char * Fglsl = NULL;// Fragment Shader
	int len1 = 0, len2 = 0;
	int vint, fint;

	Vglsl = load_shader(VEC_SHADER_FN, &len1);
	if(!Vglsl) {
		fprintf(stderr, "\n Vertex Shader Fail");
		return;
	}
	vint  = exec_shader(Vglsl, GL_VERTEX_SHADER);

	Fglsl = load_shader(FRAG_SHADER_FN, &len2);
	if(!Fglsl) {
		fprintf(stderr, "\n Vertex Shader Fail");
		return;
	}
	fint  = exec_shader(Fglsl, GL_FRAGMENT_SHADER);

	GLuint shader_prg = glCreateProgram();
	glAttachShader(shader_prg, vint);
	glAttachShader(shader_prg, fint);
	//glAttachShader(vint, shader_prg);
	//glAttachShader(fint, shader_prg);

	glLinkProgram(shader_prg);


	// initialize VAO & VBO
	VAO = 0;
	VBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);



	// ARRANGE INSTRUCTIONS FOR gl MANAGEMENT OF VERTEX STRUCT

	// 0) give the VBO the data it uses
	glBufferData(
		/* Type */		GL_ARRAY_BUFFER,
		/* Size */		sizeof(struct Vertex) * 3,
		/* Data */		triangle,
		/* Changes? */  GL_STATIC_DRAW // dynamic, stream
		);

	// 1) POSITION ATTRIBUTE
	glVertexAttribPointer(
		/* Attr, Index */ 0,		// ID = zero
		/* Num Element */ 3,		// two values--> x & y
		/* Type */ GL_FLOAT,		// (type of element)
		/* Normalize?*/ GL_FALSE,	// for a triangle, no
		/* Stride */ sizeof(struct Vertex), // dist to next ID0
		/* Offset */ (void*)0		// dist from Vertex Ptr
		);

	// 2) COLOR ATTRIBUTE (or texture)
	glVertexAttribPointer(
		/* Attr, Index */ 1,				// ID = 1
		/* Num Element */ 4,				// FOUR values--> RGBA
		/* Type */ GL_FLOAT,				// (type of element)
		/* Normalize?*/ GL_FALSE,			// for a triangle, no
		/* Stride */ sizeof(struct Vertex), // dist to next ID1 
		/* Offset */ (void*)(3 * sizeof(float)) // dist from Vertex Ptr
		);
	
	// them enable these attributes
	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	
	//printf("\nVAO = %d\nVBO = %d", VAO, VBO);

	glUseProgram(shader_prg);// THIS IS IMPORTANT
		/* The calling of the shader is very program specific.
		 * In programs where different shaders are used it is
		 * important to put this call somewhere else.
		 */


}


void Draw(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glBindVertexArray(VAO);
	glDrawArrays(
		/* Primitive */		GL_TRIANGLES,
		/* Offset */		0,
		/* numVertices */	3
		);

	glFlush();
	CheckGLErrors();// check for errors

}

void DrawLoop(int val) {

	glutPostRedisplay();
	glutTimerFunc(val, DrawLoop, val);

	
}


int main(int argc, char *argv[]) {

	// GOOD OLD FASHION HEAP VARIABLES	// CLEVER GLOBAL VARIABLE HANDLING
	struct Vertex * triangle = malloc(sizeof(struct Vertex) * 3);
	

	// INITIALIZE GLUT
	glutInit(&argc, argv);// main command
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);// for automatic cleanup purposes,
		/* otherwise requires getting window index and calling appropriate functions */
	glutInitDisplayMode(GLUT_RGBA);//inits RGB and A value usage
	glutInitWindowSize(800, 600);
	//glutWindowPosition(0, 0);// w/o inits to ??,??
	glutInitContextVersion(3, 2);// version 4.3
	glutInitContextProfile(GLUT_CORE_PROFILE);



	//char * text = NULL;
	//int size = 0;
	//text = load_shader("txt.txt", &size);
	//printf("\ntext = \"%s\"\n", text);
	//return(0);


	// WINDOW CREATION
	/* int win = */ //Should be cleaned up automatically upon exiting the main loop
	glutCreateWindow("FirstSpace");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { return EXIT_FAILURE; }
	CheckGLErrors();// check for errors



	// SET IP CALLBACK REGISTRATION
	//glutIdleFunc(Update);
	glutDisplayFunc(Draw);

				//printf("Maybe here\n");

	// INITIALIZE ENGENE
	Init_Gs(triangle);
	CheckGLErrors();// check for errors

	// START THE DRAW LOOP
	glutTimerFunc(16, DrawLoop, 16);

	// START MAIN LOOP
	glutMainLoop();
	
}
