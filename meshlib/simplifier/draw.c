/* =============================================================== */
/*
   CS175 : Computer Graphics
   Asst1 : ASST3

   Name : The Course
   Date : Oct 16, 1997

   Filename : draw.c
   Description : Draw various objects for this assignment

*/
/* =============================================================== */

#include <stdlib.h>
#include <string.h>
#include "GL/glut.h"

void drawFace()
{
  glColor3f(1.0,1.0,0); /* face */
  glPushMatrix();
  glutSolidSphere(1, 16, 12);
  
  glPushMatrix();  /* nose */
  glTranslatef(1.4,0,0);
  glScalef(1.3,1.0,1.0);
  glutSolidSphere(0.4, 16, 12);
  glPopMatrix();

  glPushMatrix(); /* Left Eye */
  glTranslatef(0.75 ,0.6 * 0.707,0.6*0.707);
  glColor3f(0,0.5,0.4);
  glutSolidSphere(0.2, 16, 8);
  glPopMatrix();
  
  glPushMatrix(); /* Right Eye */
  glTranslatef(0.75 ,0.6 *0.707, -0.6*0.707);
  glColor3f(0,0.5,0.4);
  glutSolidSphere(0.2, 16, 8);
  glPopMatrix();
  
  glPopMatrix();
}

/*---------------------------------------------------------------------------*/
void drawStar()
{
  glColor3f(1.0,0,0);
  
  glPushMatrix();
  glutSolidCube(0.6);
  
  glPushMatrix();
  glTranslatef(0,0,0.3);
  glutSolidCone(0.25, 1.2, 4, 4);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0,0,-0.3);
  glRotatef( 180, 1,0,0);
  glutSolidCone(0.25, 1.2, 4, 4);
  glPopMatrix();
  

  glPushMatrix();
  glTranslatef(0.3 ,0 ,0);
  glRotatef( 90, 0,1,0);
  glutSolidCone(0.3, 1, 4, 4);
  glPopMatrix();
  
  
  glPushMatrix();
  glTranslatef(-0.3 , 0 ,0);
  glRotatef( -90, 0,1,0);
  glutSolidCone(0.3, 1, 4, 4);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-1.2 , 0 ,0);
  glScalef(0.15,0.15,0.15);
  glutSolidDodecahedron();
  glPopMatrix();
  glPopMatrix();
}

/*---------------------------------------------------------------------------*/

void drawprops()
{
  /* Draw Heads */
  glPushMatrix();
  glTranslatef(12,0.5,0);
  glRotatef( 180, 0,1,0);
  glScalef(1.0,1.25, 1.0 );
  drawFace();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef( 12, 0.5, 6);
  glRotatef( 180, 0,1,0);
  glScalef(0.75,1.0, 1.25 );
  drawFace();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef( 12,0.5, -6);
  glRotatef( 180, 0,1,0);
  glScalef(1.25,1.0, 0.75 );
  drawFace();
  glPopMatrix();
  
  /* Draw figures */
  glPushMatrix();
  glTranslatef(-12, 0.0, 0);
  glRotatef(-90,0,0,1);
  drawStar();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-12,0.0,6.0);
  glRotatef(-90,0,0,1);
  glScalef(0.75, 1.0, 1.25 );
  drawStar();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-12,0.0,-6.0);
  glRotatef(-90,0,0,1);
  glScalef(1.25,1.0, 0.75 );
  drawStar();
  glPopMatrix();
  
  glColor3f(0,0,1.0);
  glPushMatrix();
  glTranslatef( 0,0,12);
  glutSolidDodecahedron();
  glPushMatrix();
  glTranslatef( -6, 0,0);
  glScalef( 1.25, 1.25, 1.25 );
  glutSolidDodecahedron();
  glPopMatrix();
  glPushMatrix();
  glTranslatef( 6, 0,0);
  glScalef( 0.75, 0.75 , 0.75 );
  glutSolidDodecahedron();
  glPopMatrix();
  glPopMatrix();
  
  glColor3f(0,1.0,0.0);
  glPushMatrix();
  glTranslatef( 0,0,-12);
  glutSolidIcosahedron();
  glPushMatrix();
  glTranslatef( 6, 0,0);
  glScalef( 1.25, 1.25, 1.25);
  glutSolidIcosahedron();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-6, 0,0);
  glScalef(0.75, 0.75, 0.75);
  glutSolidIcosahedron();
  glPopMatrix();
  glPopMatrix(); 
}



void drawfloor()
{
  int i;

  glDisable(GL_LIGHTING);

  
  glColor3f(0.35,.2,0.1);  //draw the ground
  glBegin(GL_POLYGON);
  
  glVertex3f( 500, -2,  500);
  glVertex3f( 500, -2, -500);
  glVertex3f(-500, -2, -500);
  glVertex3f(-500, -2,  500);
  
  glEnd();

  glColor3f(1,0,0);      //draw grids
  
  glBegin(GL_LINES);
  for( i = 0; i < 9 ; i ++ ){
    glVertex3f( 3*i - 12, -1.8, -12);
    glVertex3f( 3*i - 12, -1.8,  12);
  }
  glColor3f(0,0,1);
  for( i = 0; i < 9 ; i ++ ){
    glVertex3f( -12, -1.8, 3*i - 12);
    glVertex3f(  12, -1.8, 3*i - 12);
  }
  glEnd();
  glEnable(GL_LIGHTING); 
}

