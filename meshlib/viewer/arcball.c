#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
//#include <GL/gl.h>
#include "coords3.h"
#include "matrix4.h"
#include "arcball.h"

/*-----------------------arc ball stuff -----------------------------*/



static  double origx,origy,origz;
static  int xcenter,ycenter,rad;

//static int min()
static int  Mymin(int i,int j){
  if (i<j) return i;
  return j;
}



void lockXYtoSphere(double *x, double *y,double *z){
  double h = *x * *x + *y * *y;
  *z =  ( (h > 1.0) ? 0.0 : sqrt(1.0-h));
  if (*z==0){
    *x /=sqrt(h);
    *y /=sqrt(h);
  }
}


void setArc(double ox, double oy){
  origx = ox;
  origy = oy;
  lockXYtoSphere(&origx,&origy,&origz);
}



qrot arcTform(double nx, double ny){
  double nz = 0;
  coords3 ov;
  coords3 nv;
  coords3 qv;
  double qw;
  lockXYtoSphere(&nx,&ny, &nz);
  ov= setCoords(origx,origy,origz);
  nv= setCoords(nx,ny,nz);
//  printf("origin is %f %f %f, new is %f %f %f\n",
//	 origx,origy,origz,nx,ny,nz);
  qv = coordCross(ov,nv);
  qw = coordDot(ov,nv);
  return setQrot(qw,qv.x,qv.y, qv.z);
}




void setWinArc(int xsz, int ysz, int ox, int oy){
  double dx;
  double dy;

  ycenter = ysz /2;
  xcenter = xsz /2;
  rad = Mymin(ycenter,xcenter)*1; 
   dx = (ox-xcenter )/(double)rad;
   dy = (oy-ycenter )/(double)rad;
  setArc(dx,dy);
}

qrot  winArcTform(int nx, int ny){
  double dx = (nx-xcenter )/(double)rad;
  double dy = (ny-ycenter )/(double)rad;
  return(arcTform(dx,dy));
}  











