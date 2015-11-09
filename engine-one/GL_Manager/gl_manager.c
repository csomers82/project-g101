#include "gl_manager.h"

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


void Init_Gs(Vertex * polygon)
{	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float clc = 0.0625f;
	// (PROGRAM SPECIFIC)
	glClearColor(clc, clc, clc, 1.0f);// Background as opaque blk
		//expects GLfloats (4 byte float type)


	// initialize the color data of the polygon
	int i;
	float r = 0.35f;//(rand() % 100) / 100.0f;
	float g = 0.18f;//(rand() % 100) / 100.0f;
	float b = 0.14f;//(rand() % 100) / 100.0f;
	//printf("col = [%6.3f, %6.3f, %6.3f]\n", r, g, b);
	float temp;
	printf("\n\n");	
	for(i = 0; i < N_VERTICES; i++) 
	{
		init_Triangle(&(polygon[i*3]), r, g, b);		
		temp = r;
		r = b;
		b = g;
		g = temp;
	}


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
		/* Size */		sizeof((Vertex) * 3 * N_VERTICES,
		/* Data */		polygon,
		/* Changes? */  GL_STATIC_DRAW // dynamic, stream
		);

	// 1) POSITION ATTRIBUTE
	glVertexAttribPointer(
		/* Attr, Index */ 0,		// ID = zero
		/* Num Element */ 3,		// two values--> x & y
		/* Type */ GL_FLOAT,		// (type of element)
		/* Normalize?*/ GL_FALSE,	// for a triangle, no
		/* Stride */ sizeof(Vertex), // dist to next ID0
		/* Offset */ (void*)0		// dist from Vertex Ptr
		);

	// 2) COLOR ATTRIBUTE (or texture)
	glVertexAttribPointer(
		/* Attr, Index */ 1,				// ID = 1
		/* Num Element */ 4,				// FOUR values--> RGBA
		/* Type */ GL_FLOAT,				// (type of element)
		/* Normalize?*/ GL_FALSE,			// for a triangle, no
		/* Stride */ sizeof(Vertex), // dist to next ID1 
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

void MouseFunction(int button, int state, int x, int y)
{
	//printf("%-8s %-8s\n", (state == GLUT_DOWN)? "click,": "unclick,", 
	//	(button == GLUT_LEFT_BUTTON) ? "left": (button == GLUT_RIGHT_BUTTON)? "right": "middle"); 
	//printf("mouse position = (%5d, %5d)\n", x, y);

	X = x;
	Y = y;
	//printf("mouse position = (%5d, %5d)\n", X, Y);
	getNormCoords(x, y, &norm_X, &norm_Y);
	printf("mouse position = (%5.2f, %5.2f)\n", norm_X, norm_Y);
	int event = collision_mouse_circle(ORIG_X, ORIG_Y, RADIUS);
	printf("inside circle = \"%s\"\n", (!event)? "false": ((event == 1)? "true": "edge"));
	//char str[4] = "zap!";
	//int i;
	//gl_RasterPos2f(x,y);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//for(i = 0; i < 4; i++)
	//{
	//	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)str[i]);
	//}
	printf("\n");

	return;
}

void Passive(int x, int y)
{
	//printf("mouse position = (%d, %d)\n", x, y);
	X = x;
	Y = y;
	return;
}


void Keyboard(unsigned char k, int x, int y)
{
	printf("mouse position = (%d, %d)\tkeyboard = %c\n", x, y, k);
	X = x;
	Y = y;
	return;
}
void KeyExtras(int k, int x, int y)
{
	X = x;
	Y = y;
	if( k == GLUT_KEY_LEFT  ||
		k == GLUT_KEY_RIGHT ||
		k == GLUT_KEY_DOWN  ||
		k == GLUT_KEY_UP)
	{	if(k == GLUT_KEY_LEFT)
			printf("keyboard: < \n");
		else if(k == GLUT_KEY_RIGHT)
			printf("keyboard:   >\n");
		else if(k == GLUT_KEY_UP)
			printf("keyboard:  ^ \n");
		else
			printf("keyboard:  v \n");
	}
	return;
}



void Draw(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glBindVertexArray(VAO);
	glDrawArrays(
		/* Primitive */		GL_TRIANGLES,
		/* Offset */		0,
		/* numVertices */	3 * N_VERTICES
		);

	glFlush();
	CheckGLErrors();// check for errors

}

void DrawLoop(int val) {

	glutPostRedisplay();
	glutTimerFunc(val, DrawLoop, val);

	
}

void GL_Manager_initialize() 
{
	// INITIALIZE GLUT
	glutInit(&argc, argv);// main command
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);// for automatic cleanup purposes,
		// * otherwise requires getting window index and calling appropriate functions * /
	glutInitDisplayMode(GLUT_RGBA);//inits RGB and A value usage
	//glutWindowPosition(0, 0);// w/o inits to ??,??
	glutInitContextVersion(3, 2);// version 4.3
	//glutInitContextVersion(4, 3);// version 4.3
	glutInitContextProfile(GLUT_CORE_PROFILE);


}

void GL_Manager_create_window(char * name, int width, int length)
{
	
	glutInitWindowSize(width, length);
	glutCreateWindow(name);





}

int main(int argc, char *argv[]) {

/*	//float A[2 * 3] = {1, 0, -2, 0, 3, -1};
	//float B[3 * 2] = {0, 3, -2, -1, 0, 4};
	float A[2 * 2] = {1,2,3,4};
	float B[2 * 2] = {5,6,7,8};
	float * C = NULL;
	float * D = NULL;
	//C = matrix_multiply(A, 3, 2, B, 2, 3);
	C = matrix_multiply(A, 2, 2, B, 2, 2);
	D = matrix_multiply(B, 2, 2, A, 2, 2);
	printf("%-8s| %5.2f %5.2f |\n%8s| %5.2f %5.2f |\n\n", "AB = ", C[0], C[1], "", C[2], C[3]);
	printf("%-8s| %5.2f %5.2f |\n%8s| %5.2f %5.2f |\n\n", "BA = ", D[0], D[1], "", D[2], D[3]);
	free(C);
*/

	// GOOD OLD FASHION HEAP VARIABLES	// CLEVER GLOBAL VARIABLE HANDLING
	struct Vertex * polygon = create_regular_polygon(ORIG_X, ORIG_Y, RADIUS);
	srand(time(NULL));	

	//char * text = NULL;
	//int size = 0;
	//text = load_shader("txt.txt", &size);
	//printf("\ntext = \"%s\"\n", text);
	//return(0);

	// WINDOW CREATION
	// / * int win = * / //Should be cleaned up automatically upon exiting the main loop

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { return EXIT_FAILURE; }
	CheckGLErrors();// check for errors



	// SET IP CALLBACK REGISTRATION
	//glutIdleFunc(Update);
	glutDisplayFunc(Draw);
	glutMouseFunc(MouseFunction);// mouse downclick function
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(Passive);// mouse on screen movement
	glutSpecialFunc(KeyExtras);// arrow keys and func keys

				//printf("Maybe here\n");

	// INITIALIZE ENGENE
	Init_Gs(polygon);
	CheckGLErrors();// check for errors

	// START THE DRAW LOOP
	glutTimerFunc(16, DrawLoop, 16);

	// START MAIN LOOP
	glutMainLoop();



	free(polygon);
	return(EXIT_SUCCESS);
}
