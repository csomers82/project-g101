#include <stdio.h>
#include <stdlib.h>
#include <math.h>



float * matrix_multiply(float * mat1, int cols_m1, int rows_m1,
						float * mat2, int cols_m2, int rows_m2)
{
	// qualify the input matrices
	if(cols_m1 != rows_m2) {
		fprintf(stderr, "Error: Matrix dimensions incompatible for multiplying\n");
		return(NULL);
	}

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




struct Vector * cross_product(const struct Vector vectorA, const struct Vector vectorB)
{	/* FIRST DRAFT - SOMEWHAT TEMPORARY */
	float i = vectorA.Y * vectorB.Z - vectorA.Z * vectorB.Y;
	float j = vectorA.X * vectorB.Z - vectorA.Z * vectorB.X;
	float k = vectorA.X * vectorB.Y - vectorA.Y * vectorB.X;
	struct Vector * cross_product = vertex_create( i, j, k);
	return(cross_product);
}







