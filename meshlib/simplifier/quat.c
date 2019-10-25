#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
//#include <GL/gl.h>
#include "coords3.h"
#include "matrix4.h"
#include "quat.h"


void qrotPrint(qrot q){
  printf("quat is %f %f %f %f\n",
	 q.w, q.x, q.y, q.z);
}

qrot qrotIdent(){
  qrot res;
  res.w=1;
  res.x=0;
  res.y=0;
  res.z=0;
  return res;
}

qrot rotx2qrot(double deg){
  qrot res;
  double cs,ss;
  cs = cos( deg*PI/360.  );
  ss = sin( deg*PI/360.  );
  res.w=cs;
  res.x=ss;
  res.y=0;
  res.z=0;
  return res;
}

qrot roty2qrot(double deg){
  qrot res;
  double cs,ss;
  cs = cos( deg*PI/360.  );
  ss = sin( deg*PI/360.  );
  res.w=cs;
  res.y=ss;
  res.x=0;
  res.z=0;
  return res;
}

qrot setQrot(double a, double b, double c, double d){
  qrot res;
  res.w=a;
  res.x=b;
  res.y=c;
  res.z=d;
  return res;
}

double  quatDot(qrot a,  qrot b) { 
  double res;
  res = a.w * b.w +
    a.x * b.x +
      a.y * b.y+
	a.z * b.z;
  return res;
}




qrot quatAdd(qrot  a, qrot b){
  qrot res;
  res.x = a.x+b.x;
  res.y = a.y+b.y;
  res.z = a.z+b.z;
  res.w = a.w+b.w;
  return res;
} 
  
qrot scalarQuatMul(double s, qrot b)  {
  qrot res;
  res.w = s * b.w;
  res.x = s * b.x;
  res.y = s * b.y;
  res.z = s * b.z;
  return res;
}
  


qrot qrotInv(qrot  a){
  qrot res;
  res = a;
  res.x=(-1*a.x);
  res.y=(-1*a.y);
  res.z=(-1*a.z);
  res = scalarQuatMul(  1./quatDot(a,a) , res);
  return res;
}






qrot qrotCompose(qrot a, qrot  b){
  qrot qres;
  double sa,sb,sres;
  coords3 va,vb,vres;

  sa = a.w;
  va = setCoords(a.x, a.y, a.z);
  sb = b.w;
  vb = setCoords(b.x, b.y, b.z);
  sres = sa*sb - coordDot(va,vb);
  
  vres =   coordAdd(
		   coordAdd(
			   scalarCoordMul(sa,vb), 
			   scalarCoordMul(sb,va)),
		   coordCross(va,vb));
  qres = setQrot(sres,vres.x, vres.y, vres.z);
  return qres;
}


coords3 qrotApply(qrot q, coords3 p){
  qrot tt,pq;
  pq= setQrot(0,p.x, p.y, p.z);
  tt =     qrotCompose(q,
		       qrotCompose(pq, qrotInv(q)));
  
  return setCoords(tt.x,tt.y, tt.z);
}





matrix4  putInMatrix(qrot q){
  matrix4 m;
  double s = 2.0 / quatDot(q, q);
  double xs = q.x*s;
  double ys = q.y*s;
  double zs = q.z*s;
  
  double wx = q.w*xs;
  double wy = q.w*ys;
  double wz = q.w*zs;
  
  double xx = q.x*xs;
  double xy = q.x*ys;
  double xz = q.x*zs;
  
  double yy = q.y*ys;
  double yz = q.y*zs;
  double zz = q.z*zs;
  

  m.element[xIndex][xIndex] = 1.0 - (yy + zz);
  m.element[xIndex][yIndex] = xy - wz;
  m.element[xIndex][zIndex] = xz + wy;
  m.element[yIndex][xIndex] = xy + wz;
  m.element[yIndex][yIndex] = 1.0 - (xx + zz);
  m.element[yIndex][zIndex] = yz - wx;
  m.element[zIndex][xIndex] = xz - wy;
  m.element[zIndex][yIndex] = yz + wx;
  m.element[zIndex][zIndex] = 1.0 - (xx + yy);

  
  m.element[0][3] = 0.0;
  m.element[1][3] = 0.0;
  m.element[2][3] = 0.0;
  m.element[3][0] = m.element[3][1] = m.element[3][2] = 0.0;
  m.element[3][3] = 1.0;
  

  return m;
}


