#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "geometry.h"



void init_Vertex(Vertex * V, float x, float y, float r, float g, float b) 
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



Vertex * create_regular_polygon(float orig_x, float orig_y, float radius)
{
	Vertex * polygon = malloc(sizeof(Vertex) * 3 * N_VERTICES);
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

void init_Triangle(Vertex * T, float r, float g, float b)
{
	init_Vertex(&(T[0]), T[0].X, T[0].Y,  r,  g,  b);
	init_Vertex(&(T[1]), T[1].X, T[1].Y,  r,  g,  b);
	init_Vertex(&(T[2]), T[2].X, T[2].Y,  r,  g,  b);

}


Vertex * create_axis_aligned_rectangle(float x, float y, float h, float w)
{
	float hhalf = h / 2.00f;
	float whalf = w / 2.00f;


	// ensure that the box will not have any component that is off of the screen

	// in the X direction: center +/- w/2
	if((x - whalf < 0.0f) || (x + whalf > 0.0f)) 
		return NULL;

	// in the Y direction: center +/- h/2
	if((y - hhalf < 0.0f) || (y + hhalf > 1.0f)) 
		return NULL;


	Vertex * box = malloc(sizeof(Vertex) * 6);
	int i = 0;

	// set all vertex properties to 0.0f 
	memset(box, 0, sizeof((Vertex)*6);

	box[i].X = x + whalf;// top right corner
	box[i].Y = y + hhalf;//			.	
	box[i].Z = 0.0f;	 // 
	++i;
	box[i].X = x - whalf;// top left corner
	box[i].Y = y + hhalf;//		.....
	box[i].Z = 0.0f;	 //
	++i;
	box[i].X = x - whalf;// bot left corner 
	box[i].Y = y - hhalf;//		....
	box[i].Z = 0.0f;	 //     |    
	++i;
	box[i].X = x - whalf;// bot left corner 
	box[i].Y = y - hhalf;//
	box[i].Z = 0.0f;     //     .
	++i;
	box[i].X = x + whalf;// bot right corner
	box[i].Y = y - hhalf;//
	box[i].Z = 0.0f;	 //		....
	++i;
	box[i].X = x + whalf;// top right corner
	box[i].Y = y + hhalf;//		   .
	box[i].Z = 0.0f;	 //		...|

	return(box);
}


