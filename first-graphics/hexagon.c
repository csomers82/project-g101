#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define DEBUG
#ifdef DEBUG
	#define SHOUT(x) printf("%d", x)
#endif

#define VEC_SHADER_FN "vertexShader.glsl"
#define FRAG_SHADER_FN "fragmentShader.glsl"

#define N_VERTICES 24


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

//.43 .27 .28

//struct Vertex * triangle;

// ===========================================================================
/* It is important to note that in this file matricies are treated a unidimensional
 * arrays. Matricies as nested argigate types cannot be returned from functions
 * and though multi-level pointer structure has a similar meaning it has a fundamentally
 * different memory layout. Thus, to index a matrix, a row's values are in order
 * then followed by the values of next row in order and so on. To reference a 
 * position a row reference and a number of elements er row is required. Thus the 
 * 6th element of a 3x3 array would be
 *			
 *		matrix[row*epr + x]; = 5;	
 *		
 *		or index 3, (x = 2), in the 2nd row, (row = 1), 
 *		when there are 3 elements per row, (epr = 3)
 */



void init_Vertex(struct Vertex * V, float x, float y, float r, float g, float b) 
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

float * matrix_multiply(float * mat1, int cols_m1, int rows_m1,
						float * mat2, int cols_m2, int rows_m2)
{
	// qualify the input matrices
	if(cols_m1 != rows_m2) {
		fprintf(stderr, "Error: Matrix dimensions incompatible for multiplying\n");
		return(NULL);
	}
	//if(cols_m1 < 2 || rows_m1 < 2 || cols_m2 < 2 || rows_m2 < 2) {
	//	fprintf(stderr, "Error: Parameters indicate a scalar or negative valued dimension\n");
	//	return(NULL);
	//}

	int i, j, k;
	float * sum = malloc(sizeof(float) * rows_m1 * cols_m2);	
	// initialize values all result-matrix values to zero
	for(i = 0; i < rows_m1; i++) {
		for(j = 0; j < cols_m2; j++) {
			sum[i*cols_m2 + j] = 0;
		}
	}
	//printf("%-8s| %5.2f %5.2f |\n%8s| %5.2f %5.2f |\n\n", "sum = ", sum[0], sum[1], "", sum[2], sum[3]);
	// multiply each elem in each row 
	for(i = 0; i < rows_m1; i++) {
		//printf("\n");
		for(j = 0; j < cols_m2; j++	) {
			for(k = 0; k < rows_m2; k++) {// can be about rows_m2 or cols_m1 (def'd as equal)

				sum[i*cols_m2 + j] += mat1[i*cols_m1+ k] * mat2[k*cols_m2 + j];
				//printf("i, j, k = {%d, %d, %d}\tmat1[%d] = %4.1f\tmat2[%d] = %4.1f\tvalue = %5.2f\t sum[i][j]=%5.2f\n", 
				//		i, j, k, i*rows_m1+ k,  mat1[i*rows_m1+ k], k*cols_m1 + j,
				//		mat2[k*cols_m2 + j], mat1[i*rows_m1+ k] * mat2[k*cols_m2 + j] ,
				//		sum[i*cols_m2 + j]);
			}
		}
	}
	//printf("\n");
	return(sum);
}

float * affine_transformation_2d(float local_orig[2], float reference[2], float rotation[4])
{
	float * matrix;// [3][3] product term 1: cool part of affine transformation
	float * pterm;//  [3][1] product term 2: reference point that is rotating

	float * result;// [3][1]
	float * output;
	//frame reference matrix for multiplication
	matrix = malloc(sizeof(float) * 9);
	int row = 0, cpr = 3;// cpr = columns per row
	matrix[row*cpr + 0] = rotation[0];// rot[0][0]
	matrix[row*cpr + 1] = rotation[1];// rot[0][1]
	matrix[row*cpr + 2] = local_orig[0];// origin x
	row += 1;
	matrix[row*cpr + 0] = rotation[2];// rot[1][0]
	matrix[row*cpr + 1] = rotation[3];// rot[1][1]
	matrix[row*cpr + 2] = local_orig[1];// origin y
	row += 1;
	matrix[row*cpr + 0] = 0.0f;
	matrix[row*cpr + 1] = 0.0f;
	matrix[row*cpr + 2] = 1.0f;

	//frame product term for multiplication
	pterm = malloc(sizeof(float) * 3);
	pterm[0] = reference[0];
	pterm[1] = reference[1];
	pterm[2] = 1.0f;// signifies a vector not a direction

	result = matrix_multiply(matrix, 3, 3, pterm, 1, 3);
	output = malloc(sizeof(float) * 2);
	output[0] = result[0];
	output[1] = result[1];
	free(result);
	free(matrix);
	free(pterm);
	return(output);
}


struct Vertex * create_regular_polygon(float orig_x, float orig_y, float radius)
{
	struct Vertex * polygon = malloc(sizeof(struct Vertex) * 3 * N_VERTICES);
	double delta_theta = (M_PI) / (N_VERTICES / 2.0000000f);
	double angle;// rotation about the normal angle
	int vertex_i;
	float local_orig[2] = {orig_x, orig_y};
	float first[2] = {orig_x + radius, orig_y + 0.0f};
	float rotation[4] = {1, 0, 0, 1};
	float * result = NULL;
	float * xcoords = malloc(sizeof(float) * N_VERTICES);
	float * ycoords = malloc(sizeof(float) * N_VERTICES);

	for(vertex_i = 0; vertex_i < N_VERTICES; vertex_i++)
	{	
		angle = vertex_i * delta_theta;
		rotation[0] = cos(angle);// (0, 0)	
		rotation[1] = sin(angle) * -1;	// (0, 1)
		rotation[2] = sin(angle);// (1, 0)
		rotation[3] = cos(angle);// (1, 1)	
		
		result = affine_transformation_2d(local_orig, first, rotation);
		//printf("result[0] = %5.3f\t result[1] = %5.3f\n", result[0], result[1]);
		xcoords[vertex_i] = result[0];
		ycoords[vertex_i] = result[1];
		free(result);
	}
	float z = 0.5f;
	for(vertex_i = 0; vertex_i < N_VERTICES; vertex_i++)
	{	
		int B = (vertex_i) % (N_VERTICES);
		int C = (vertex_i + (N_VERTICES - 1)) % (N_VERTICES);
		//printf("\ncoords[0] = %5.3f\t coords[1] = %5.3f\n", xcoords[C], ycoords[C]);
		//printf("coords[0] = %5.3f\t coords[1] = %5.3f\n", xcoords[B], ycoords[B]);
		init_Vertex(&(polygon[vertex_i * 3 + 0]), orig_x, orig_y,			z,z,z);
		init_Vertex(&(polygon[vertex_i * 3 + 1]), xcoords[C], ycoords[C],	z,z,z);	
		init_Vertex(&(polygon[vertex_i * 3 + 2]), xcoords[B], ycoords[B],	z,z,z);
	}
	free(xcoords);
	free(ycoords);
	printf("Polygon = %p\n", polygon);
	int i;
	return(polygon);
}

void init_Triangle(struct Vertex * T, float r, float g, float b)
{
	init_Vertex(&(T[0]), T[0].X, T[0].Y,  r,  g,  b);
	init_Vertex(&(T[1]), T[1].X, T[1].Y,  r,  g,  b);
	init_Vertex(&(T[2]), T[2].X, T[2].Y,  r,  g,  b);

}


void Init_Gs(struct Vertex * polygon)
{	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	float clc = 0.0625f;
	// (PROGRAM SPECIFIC)
	glClearColor(clc, clc, clc, 1.0f);// Background as opaque blk
		//expects GLfloats (4 byte float type)


	// initialize the color data of the polygon
	int i;
	float r = (rand() % 100) / 100.0f;
	float g = (rand() % 100) / 100.0f;
	float b = (rand() % 100) / 100.0f;
	printf("col = [%6.3f, %6.3f, %6.3f]\n", r, g, b);
	float temp;
	int stride = sizeof(struct Vertex)*3;
	printf("\n\n");	
	for(i = 0; i < N_VERTICES; i++) 
	{
		init_Triangle(&(polygon[i*3]), r, g, b);		
		temp = r;
		r = b;
		b = g;
		g = temp;
	}
/*		
	for(i = 0; i < N_VERTICES*3; i++) 
	{	printf("Polygon[%2d]|\tpos = [%6.3f, %6.3f, %6.3f]\t col = [%6.3f, %6.3f, %6.3f]\n",
			i, polygon[i].X,  polygon[i].Y,  polygon[i].Z,
			     polygon[i].R, polygon[i].G, polygon[i].B);	
		if(i % 3 == 2) printf("\n");

	}
	printf("\n\n");
*/


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
		/* Size */		sizeof(struct Vertex) * 3 * N_VERTICES,
		/* Data */		polygon,
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
		/* numVertices */	3 * N_VERTICES
		);

	glFlush();
	CheckGLErrors();// check for errors

}

void DrawLoop(int val) {

	glutPostRedisplay();
	glutTimerFunc(val, DrawLoop, val);

	
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
	struct Vertex * polygon = create_regular_polygon(0.0f, 0.0f, 1.0f);
	srand(time(NULL));	

	// INITIALIZE GLUT
	glutInit(&argc, argv);// main command
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);// for automatic cleanup purposes,
		// * otherwise requires getting window index and calling appropriate functions * /
	glutInitDisplayMode(GLUT_RGBA);//inits RGB and A value usage
	glutInitWindowSize(800, 800);
	//glutWindowPosition(0, 0);// w/o inits to ??,??
	glutInitContextVersion(3, 2);// version 4.3
	glutInitContextProfile(GLUT_CORE_PROFILE);



	//char * text = NULL;
	//int size = 0;
	//text = load_shader("txt.txt", &size);
	//printf("\ntext = \"%s\"\n", text);
	//return(0);


	// WINDOW CREATION
	// / * int win = * / //Should be cleaned up automatically upon exiting the main loop
	glutCreateWindow("FirstSpace");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { return EXIT_FAILURE; }
	CheckGLErrors();// check for errors



	// SET IP CALLBACK REGISTRATION
	//glutIdleFunc(Update);
	glutDisplayFunc(Draw);

				//printf("Maybe here\n");

	// INITIALIZE ENGENE
	Init_Gs(polygon);
	CheckGLErrors();// check for errors

	// START THE DRAW LOOP
	glutTimerFunc(16, DrawLoop, 16);

	// START MAIN LOOP
	glutMainLoop();



	free(polygon);
}
