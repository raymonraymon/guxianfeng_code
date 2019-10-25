/* =============================================================== */
/*
   CS175 : Computer Graphics
   Asst1 : ASST3

   Name : The Course
   Date : Oct 16, 1998

   Filename : matrix4.c
   Description : matrix class

*/
/* =============================================================== */

#include "matrix4.h"

int affine_matrix4_inverse (matrix4 * in, matrix4 * out)
{
  register  double    det_1;
  double    pos, neg, temp;
  
#define ACCUMULATE  \
  if (temp >= 0.0)  \
      pos += temp;  \
  else              \
      neg += temp;

#define PRECISION_LIMIT (1.0e-15)

  /*
   * Calculate the determinant of submatrix A and determine if the
   * the matrix is singular as limited by the double precision
   * floating-point data representation.
   */
  pos = neg = 0.0;
  temp =  in->element[0][0] * in->element[1][1] * in->element[2][2];
  ACCUMULATE
    temp =  in->element[0][1] * in->element[1][2] * in->element[2][0];
  ACCUMULATE
    temp =  in->element[0][2] * in->element[1][0] * in->element[2][1];
  ACCUMULATE
    temp = -in->element[0][2] * in->element[1][1] * in->element[2][0];
  ACCUMULATE
    temp = -in->element[0][1] * in->element[1][0] * in->element[2][2];
  ACCUMULATE
    temp = -in->element[0][0] * in->element[1][2] * in->element[2][1];
  ACCUMULATE
    det_1 = pos + neg;
  
  /* Is the submatrix A singular? */
  if ((det_1 == 0.0) || (ABS(det_1 / (pos - neg)) < PRECISION_LIMIT)) {
    
    /* Matrix M has no inverse */
    fprintf (stderr, "affine_matrix4_inverse: singular matrix\n");
    return FALSE;
  }
  
  else {
    
    /* Calculate inverse(A) = adj(A) / det(A) */
    det_1 = 1.0 / det_1;
    out->element[0][0] =   ( in->element[1][1] * in->element[2][2] -
			     in->element[1][2] * in->element[2][1] ) * det_1;
    out->element[1][0] = - ( in->element[1][0] * in->element[2][2] -
			     in->element[1][2] * in->element[2][0] ) * det_1;
    out->element[2][0] =   ( in->element[1][0] * in->element[2][1] -
			     in->element[1][1] * in->element[2][0] ) * det_1;
    out->element[0][1] = - ( in->element[0][1] * in->element[2][2] -
			     in->element[0][2] * in->element[2][1] ) * det_1;
    out->element[1][1] =   ( in->element[0][0] * in->element[2][2] -
			     in->element[0][2] * in->element[2][0] ) * det_1;
    out->element[2][1] = - ( in->element[0][0] * in->element[2][1] -
			     in->element[0][1] * in->element[2][0] ) * det_1;
    out->element[0][2] =   ( in->element[0][1] * in->element[1][2] -
			     in->element[0][2] * in->element[1][1] ) * det_1;
    out->element[1][2] = - ( in->element[0][0] * in->element[1][2] -
			     in->element[0][2] * in->element[1][0] ) * det_1;
    out->element[2][2] =   ( in->element[0][0] * in->element[1][1] -
			     in->element[0][1] * in->element[1][0] ) * det_1;
    
    /* Calculate -C * inverse(A) */
    out->element[0][3] = - ( in->element[0][3] * out->element[0][0] +
			     in->element[1][3] * out->element[0][1] +
			     in->element[2][3] * out->element[0][2] );
    out->element[1][3] = - ( in->element[0][3] * out->element[1][0] +
			     in->element[1][3] * out->element[1][1] +
			     in->element[2][3] * out->element[1][2] );
    out->element[2][3] = - ( in->element[0][3] * out->element[2][0] +
			     in->element[1][3] * out->element[2][1] +
			     in->element[2][3] * out->element[2][2] );
    
    /* Fill in last column */
    out->element[3][0] = out->element[3][1] = out->element[3][2] = 0.0;
    out->element[3][3] = 1.0;

    return TRUE;
  }
}


/*------------------------------------------------------------------------*/
void Test( GLdouble * obj, GLdouble * inv )
{  
  GLdouble Mat[16];
  int i,j;
  
  glPushMatrix();
  glLoadIdentity();
  glMultMatrixd( (GLdouble *) obj );
  glMultMatrixd( (GLdouble *) inv );
  glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *)Mat);
  for( i = 0; i < 4; i ++ ){
    for( j = 0; j < 4; j ++ ) fprintf(stderr,"%f  ", *(Mat+i*4+j));
    fprintf(stderr,"\n");
  }
  fprintf(stderr,"\n\n");
} 

/*-------------------------------------------------------------------------*/

matrix4  matInverse(matrix4 in)
{
  matrix4 out;
  affine_matrix4_inverse(&in,&out);
  return out;
} 



void mat2GL(matrix4 m, GLdouble *g){
  int i,j;
  for( i = 0; i < 4; i ++ )
    for( j = 0; j < 4; j ++ ) 
	*( g + i*4 + j )= m.element[j][i];
}


void   GL2matPP(GLdouble *g, matrix4 * out){
  int i,j;
  for( i = 0; i < 4; i ++ )
    for( j = 0; j < 4; j ++ ) 
      out->element[j][i] = *( g + i*4 + j );

}
matrix4  GL2mat(GLdouble *g){
  matrix4 out;
  int i,j;
  for( i = 0; i < 4; i ++ )
    for( j = 0; j < 4; j ++ ) 
      out.element[j][i] = *( g + i*4 + j );

  return out;
}


matrix4 matMatMul(matrix4 a, matrix4 b){
  matrix4 out;
  int i,j,k;
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      out.element[i][j]=0;
      for (k=0; k<4; k++){
	out.element[i][j] += 
	  a.element[i][k] *
	  b.element[k][j] ; 
	
      }
    }
  }
  return out;
}

/* only works for two affine matrices */
void matMatMulppp(matrix4 *a, matrix4 *b, matrix4 *out){
  int i,j,k;
  out->element[3][0] =
    out->element[3][1] =
    out->element[3][2] = 0;
  out->element[3][3] = 1;

  for (i=0; i<3; i++){
    for (j=0; j<3; j++){
      out->element[i][j]=0;
      for (k=0; k<3; k++){
	out->element[i][j] += 
	  a->element[i][k] *
	  b->element[k][j] ; 
      }
    }
    out->element[i][3]=0;
    for (k=0; k<3; k++){
      out->element[i][3] += 
	a->element[i][k] *
	b->element[k][3] ; 
      
    }
    
    
    out->element[i][3] += 
      a->element[i][3];	
  }
}

coords3 matCoords3Mul(matrix4 m, coords3 c){
  coords3 out;
  out.x = 
    m.element[0][0] * c.x + 
    m.element[0][1] * c.y + 
    m.element[0][2] * c.z + 
    m.element[0][3] * 1; 

  out.y = 
    m.element[1][0] * c.x + 
    m.element[1][1] * c.y + 
    m.element[1][2] * c.z + 
    m.element[1][3] * 1; 

  out.z = 
    m.element[2][0] * c.x + 
    m.element[2][1] * c.y + 
    m.element[2][2] * c.z + 
    m.element[2][3] * 1; 

  return out;
}

matrix4  rotx2mat(double deg){
  matrix4 out;
  
  double cs, sn;

  cs = cos( deg*2*PI/360.  );
  sn = sin( deg*2*PI/360.  );

  out.element[0][0] = 1;
  out.element[0][1] = 0;
  out.element[0][2] = 0;
  out.element[0][3] = 0;

  out.element[1][0] = 0;
  out.element[1][1] =  cs;
  out.element[1][2] = -sn;
  out.element[1][3] = 0;

  out.element[2][0] = 0;
  out.element[2][1] = sn;
  out.element[2][2] = cs;
  out.element[2][3] = 0;

  out.element[3][0] = 0;
  out.element[3][1] = 0;
  out.element[3][2] = 0;
  out.element[3][3] = 1;
  return out;
}

matrix4  roty2mat(double deg){
  matrix4 out;
  
  double cs, sn;

  cs = cos( deg*2*PI/360.  );
  sn = sin( deg*2*PI/360.  );

  out.element[0][0] = cs;
  out.element[0][1] = 0;
  out.element[0][2] = sn;
  out.element[0][3] = 0;

  out.element[1][0] = 0;
  out.element[1][1] = 1;
  out.element[1][2] = 0;
  out.element[1][3] = 0;

  out.element[2][0] = -sn;
  out.element[2][1] = 0;
  out.element[2][2] = cs;
  out.element[2][3] = 0;

  out.element[3][0] = 0;
  out.element[3][1] = 0;
  out.element[3][2] = 0;
  out.element[3][3] = 1;
  return out;
}

matrix4 ident(){
  matrix4 out;
  
  out.element[0][0]=1;
  out.element[0][1]=0;
  out.element[0][2]=0;
  out.element[0][3]=0;

  out.element[1][0]=0;
  out.element[1][1]=1;
  out.element[1][2]=0;
  out.element[1][3]=0;

  out.element[2][0]=0;
  out.element[2][1]=0;
  out.element[2][2]=1;
  out.element[2][3]=0;

  out.element[3][0]=0;
  out.element[3][1]=0;
  out.element[3][2]=0;
  out.element[3][3]=1;

  return out;
}


void printMat(matrix4 out){
  printf("%f %f %f %f\n",
	 out.element[0][0],
	 out.element[0][1],
	 out.element[0][2],
	 out.element[0][3]);
  printf("%f %f %f %f\n",
	 out.element[1][0],
	 out.element[1][1],
	 out.element[1][2],
	 out.element[1][3]);
  printf("%f %f %f %f\n",
	 out.element[2][0],
	 out.element[2][1],
	 out.element[2][2],
	 out.element[2][3]);
  printf("%f %f %f %f\n",
	 out.element[3][0],
	 out.element[3][1],
	 out.element[3][2],
	 out.element[3][3]);
}




matrix4  transpose_mat(matrix4 in){
  matrix4 out;
  int i,j;

  for( i = 0; i < 4; i ++ )
    for( j = 0; j < 4; j ++ ) out.element[i][j] = in.element[j][i];
  return out;
}

















void matCoords3Mulppp(matrix4 *m, double x, double y, double z, coords3 * out){
  out->x = 
    m->element[0][0] * x + 
    m->element[0][1] * y + 
    m->element[0][2] * z + 
    m->element[0][3] ; 

  out->y = 
    m->element[1][0] * x + 
    m->element[1][1] * y + 
    m->element[1][2] * z + 
    m->element[1][3] ; 

  out->z = 
    m->element[2][0] * x + 
    m->element[2][1] * y + 
    m->element[2][2] * z + 
    m->element[2][3] ; 

}
