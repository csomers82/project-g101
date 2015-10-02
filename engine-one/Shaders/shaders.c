#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>



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
	return(-1);
}

