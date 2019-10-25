#if !defined (_QUAT_H)
#define _QUAT_H

#include "matrix4.h"


//typedef double mat44[4][4];

#define xIndex 0
#define yIndex 1
#define zIndex 2


typedef struct qrot{
  double w,x,y,z;
}qrot;



coords3  qrotApply(qrot q, coords3 p);
qrot  setQrot(double a, double b, double c, double d);
qrot  qrotInv(qrot  a);
qrot  quatAdd(qrot  a, qrot b);
qrot  scalarQuatMul(double s, qrot b);
double  quatDot(qrot a,  qrot b);
qrot  qrotCompose(qrot a, qrot  b);
matrix4   putInMatrix(qrot q);
qrot qrotIdent();
qrot rotx2qrot(double d);
qrot roty2qrot(double d);
void qrotPrint(qrot q);


#endif 
