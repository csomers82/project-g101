#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


#define VERTEX_SHADER_FN "vertexShader.glsl"
#define FRAGMENT_SHADER_FN "fragmentShader.glsl"


#ifndef EO_SHADERS
#define EO_SHADERS


// ========================================================
/* The load_shader function reads in an entire file as a
 * character array of size 'len'. If the filename pointed
 * to by 'name' is invalid the function returns 'NULL' */
// --------------------------------------------------------
char * load_shader(char * name, int * len) ;

int exec_shader(char* glsl, GLenum compilertype );



#endif
