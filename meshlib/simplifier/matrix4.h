/* =============================================================== */
/*
   CS175 : Computer Graphics
   Asst1 : ASST3

   Name : The Course
   Date : Oct 16, 1997

   Filename : matrix4.h
   Description : Compute matrix inverses

*/
/* =============================================================== */

#ifndef __MAT4_H
#define __MAT4_H

#include "GL/glut.h"
#include <stdio.h>
#include <math.h>
#include "coords3.h"

/********************private stuff **************************/
#define ABS(a)		(((a)<0) ? -(a) : (a))
#define FALSE 0
#define TRUE 1


typedef struct matrix4{	/* 4-by-4 matrix */
  double element[4][4];
} matrix4;

void Test( GLdouble * obj, GLdouble * inv );



/********************* the interface *************************/
matrix4 ident();

matrix4 matMatMul(matrix4 a, matrix4 b);
void  matMatMulppp(matrix4 *a, matrix4 *b, matrix4 *res);

coords3 matCoords3Mul(matrix4 m, coords3 c);

matrix4 matInverse(matrix4 a);

void mat2GL(matrix4 m, GLdouble *g);
matrix4  GL2mat(GLdouble *a);

matrix4  rotx2mat(double deg);
matrix4  roty2mat(double deg);

matrix4  transpose_mat(matrix4 in);
void printMat(matrix4 a);
void matCoords3Mulppp(matrix4 *m, double x, double y, double z, coords3 * out);
void GL2matPP(GLdouble *, matrix4 *);
int affine_matrix4_inverse (matrix4 * in, matrix4 * out);

#endif /* __MAT_H */


