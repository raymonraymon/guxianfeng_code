#if !defined (_C3_H)
#define _C3_H

#define PI 3.1415927
#define RAD2DEG 360./(2*PI)



/* a 3 dimensional vector */
typedef struct coords3{
  double x,y,z;
}coords3;


coords3 setCoords(double a, double b, double c);


/* multiply scalar times a vector */
coords3 scalarCoordMul(double a, coords3 v);

/* add two coordinate vectors */
coords3 coordAdd(coords3 a, coords3 b);

/* subtract  two coordinate  vectors */
coords3 coordSub(coords3 a, coords3 b);

/* dot product */
double coordDot(coords3 a, coords3 b);

/* cross product */
coords3 coordCross(coords3 a, coords3 b);


#endif
