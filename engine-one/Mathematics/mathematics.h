#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef EO_MATH
#define EO_MATH


float * matrix_multiply(float * mat1, int cols_m1, int rows_m1,
						float * mat2, int cols_m2, int rows_m2)

float * affine_transformation_2d(float local_orig[2], float reference[2], float rotation[4])

struct Vector * cross_product(const struct Vector vectorA, const struct Vector vectorB)




#endif

