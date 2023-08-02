#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertices_matrix {
  double *matrix;
  int rows;
  double max_vertex;
} VerticesMatrix;

void rotate_x(VerticesMatrix *data, double angle);
void rotate_y(VerticesMatrix *data, double angle);
void rotate_z(VerticesMatrix *data, double angle);
void s21_scale(VerticesMatrix *V, double scl);
void s21_move_3d(VerticesMatrix *data, double move_x, double move_y,
                 double move_z);

#endif  // TRANSFORMATIONS_H