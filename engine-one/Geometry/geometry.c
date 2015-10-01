#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>




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



struct Vertex * create_regular_polygon(float orig_x, float orig_y, float radius)
{
	struct Vertex * polygon = malloc(sizeof(struct Vertex) * 3 * N_VERTICES);
	double delta_theta = (M_PI) / (N_VERTICES / 2.0000000f);
	double angle;// rotation about the normal angle
	int vertex_i;
	//float local_orig[2] = {orig_x, orig_y};
	float local_orig[2] = {0.0f, 0.0f};
	float first[2] = {0.0f + radius, 0.0f + 0.0f};
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
		init_Vertex(&(polygon[vertex_i * 3 + 1]), orig_x + xcoords[C], orig_y + ycoords[C],	z,z,z);	
		init_Vertex(&(polygon[vertex_i * 3 + 2]), orig_x + xcoords[B], orig_y + ycoords[B],	z,z,z);
	}
	free(xcoords);
	free(ycoords);
	printf("Polygon = %p\n", polygon);
	return(polygon);
}

void init_Triangle(struct Vertex * T, float r, float g, float b)
{
	init_Vertex(&(T[0]), T[0].X, T[0].Y,  r,  g,  b);
	init_Vertex(&(T[1]), T[1].X, T[1].Y,  r,  g,  b);
	init_Vertex(&(T[2]), T[2].X, T[2].Y,  r,  g,  b);

}


