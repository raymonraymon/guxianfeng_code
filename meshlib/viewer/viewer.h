/* =============================================================== */
/*
   CS175 : Computer Graphics
   Asst  : ASST3

   Name : The Course
   Date : Oct 16, 1997

   Filename : draw.h
   Description : Includes for drawing functions

*/
/* =============================================================== */

#ifndef __ASST3_H
#define __ASST3_H
#include <stdio.h>
#include "GL/glut.h"
#include "matrix4.h"
#include "arcball.h"
#include "../lib/mesh.h"
#include "../lib/funcs.h"
#include "draw.h"


/*--------------------------------------------------------------------------
  Operations
  ---------------------------------------------------------------------------*/
#define  NULL_OPERATION       0
#define  TURN                 1
#define  FORWARD              2
#define  JUMP                 4
#define  MANIPULATE_JOINT     3
/*---------------------------------------------------------------------------
  Relative to Which Coordinate System
 ---------------------------------------------------------------------------*/
#define  RELATIVE_TO_LOCAL  0
#define  RELATIVE_TO_WORLD  1
#define  RELATIVE_TO_OTHER  2
#define  RELATIVE_TO_SKYCAM 3
/*--------------------------------------------------------------------------
  Define which object is being operated
  ---------------------------------------------------------------------------*/
#define  OBJECTR 0
#define  OBJECTG 1
#define  SKYCAM 2
/*--------------------------------------------------------------------------
  Define which coordinate system is the current one
  ---------------------------------------------------------------------------*/
#define  OBJECTR_COORD 0
#define  OBJECTG_COORD 1
#define  SKYCAM_COORD  2
#define  WORLD_COORD   3
/*-------------------------------------------------------------------------
  Define view point
  --------------------------------------------------------------------------*/
#define  VIEW_FROM_WORLD   0
#define  VIEW_FROM_SKYCAM  1

#endif /* __ASST3_H */

