#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "coords3.h"

/*---------------------------------------------------------------------------

  Utilities for manipulation of vector

---------------------------------------------------------------------------*/

coords3 setCoords(double x, double y, double z){
  coords3 res;
  res.x = x;
  res.y = y;
  res.z = z;
  return res;
}



coords3 coordAdd(coords3 a, coords3 b){
  coords3 res;
  res.x = a.x + b.x;
  res.y = a.y + b.y;
  res.z = a.z + b.z;
  return res;
}

coords3 coordSub(coords3 a, coords3 b){
  coords3 res;
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;
  return res;
}


double  coordDot(coords3 a, coords3 b) { 
  double res;
  res = a.x * b.x +
    a.y * b.y +
      a.z * b.z;
  return res;
}

coords3 coordCross(coords3 v1,coords3 v2) { 
  coords3 ret;
  ret.x = v1.y*v2.z - v1.z*v2.y;
  ret.y = v1.z*v2.x - v1.x*v2.z;
  ret.z = v1.x*v2.y - v1.y*v2.x;
  return ret;
}



coords3 scalarCoordMul(double s,coords3 b)  {
  coords3 res;
  res.x = s * b.x;
  res.y = s * b.y;
  res.z = s * b.z;
  return res;
}
