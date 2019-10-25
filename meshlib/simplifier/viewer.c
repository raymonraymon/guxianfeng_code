/* =============================================================== */
/*
   CS276r : Computer Graphics

   Filename : viewer.c
   Description : viewer program for mesh lib

*/
/* =============================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "viewer.h"


Solid * solid;
Node  * hlist;
Node  * list;
Node  * halfedge_list;
Node  * merged_halfedge_list;

/*-------------------------------------------------------------------------

  For Debugging purpose 

-------------------------------------------------------------------------*/
HalfEdge * nextMergedHalfEdge = 0;


GLfloat green_mat[] = {1.f, .5f, 0.f, .5f};

int   display_mode = 0;
int   display_next_merged_halfedge = 0;
int   start_flag = 0;

/*--------------------------------------------------------------------------
  Global Variables for current state
  ---------------------------------------------------------------------------*/
int Object ;  /* which object is being operated */
int View;     /* view point */
int Mode;     /* updating mode */
int Coordnate;/* which coordinate system */
int Operation;/* which type of operation */





/*-------------------------------------------------------------------------
  Global Variables for the transformations of the objects
  --------------------------------------------------------------------------*/

qrot objGreenRotMat;
coords3 objGreenTransCoords;

qrot  skyRotMat;
coords3 skyTransCoords;


matrix4  tempRotMat;

/*-------------------------------------------------------------------------*/

int GW;
int GH;

float AR;
GLfloat lightOnePosition[] = {-.3,  1.0, -0.5, 0.0}; 
GLfloat lightTwoPosition[] = { 1,  0, 1, 1.0};
GLfloat lightOneColor[] = {0.9, 0.9, 0.9, 1};
GLfloat lightTwoColor[] = {1.0, 1.0, 0.5, 1};

GLfloat globalAmb[] = {.1, .1, .1, 1};
GLfloat globalDif[] = {.8, .8, .8, 1};




/*--------------------------------------------------------------------------
  Display
  Function
  --------------------------------------------------------------------------*/

void displayStructure( float r, float g, float b,int type, Vertex * root){
  HalfEdge *head,*the;
  Vertex * tv1, *tv2, *tv3, *vert, *tv;
  Node * node;
  Face   * f, *hf;

  glColor3f(r,g,b);
  
  if(!root->alivev)
    return;
  
  head = VertexFirstOutHalfEdge(root);
  
  switch(type){
  case 0:
    the  = head;
    do{
      
      tv1 = HalfEdgeStartVertex(the);
      tv2 = HalfEdgeEndVertex(the);
      
      glBegin(GL_LINES);
      
      glVertex3f(tv1->vcoord[0],
		 tv1->vcoord[1],
		 tv1->vcoord[2]);
      
      glVertex3f(tv2->vcoord[0],
		 tv2->vcoord[1],
		 tv2->vcoord[2]);
      glEnd();
      
      
      the = VertexNextOutHalfEdge(the);
    }while( the != head );
    break;
  case 1:
    the  = head;
    do{
      
      tv1 = HalfEdgeStartVertex(the);
      tv2 = HalfEdgeEndVertex(the);
      
      glBegin(GL_LINES);
      
      glVertex3f(tv1->vcoord[0],
		 tv1->vcoord[1],
		 tv1->vcoord[2]);
      
      glVertex3f(tv2->vcoord[0],
		 tv2->vcoord[1],
		 tv2->vcoord[2]);
      glEnd();
      
      
      the = VertexNextInHalfEdge(the);
    }while( the != head );
    break;
    
  case 2:
    hf = VertexFirstFace( root );
    f  = hf;
    do{
      tv1 = f->floop->ledges->hvert;
      tv2 = f->floop->ledges->next->hvert;
      tv3 = f->floop->ledges->prev->hvert;
      
      glBegin(GL_TRIANGLES);
      glVertex3f(tv1->vcoord[0],
		 tv1->vcoord[1],
		 tv1->vcoord[2]);
      
      glVertex3f(tv2->vcoord[0],
		 tv2->vcoord[1],
		 tv2->vcoord[2]);
      
      glVertex3f(tv3->vcoord[0],
		 tv3->vcoord[1],
		 tv3->vcoord[2]);

      
      glEnd();
      f = VertexNextFace( root, f );
    }while( f != hf );
    
    
    break;
    
  case 3:
    glPushMatrix();
      glColor3f(0,0,1);
      glTranslated(root->vcoord[0], root->vcoord[1], root->vcoord[2]);
      glutSolidSphere(0.05,16,16);
      glColor3f(r,g,b);
      glPopMatrix();		
      vert = VertexFirstVertex( root );
      
      do{
	
	glPushMatrix();
	glTranslated(vert->vcoord[0], vert->vcoord[1], vert->vcoord[2]);
	glutSolidSphere(0.035,16,16);
	glPopMatrix();		      
      vert = VertexNextVertex( root, vert );
      }while( vert != VertexFirstVertex(root));
      break;
  case 4:
    node = list;
    glPushMatrix();
    glColor3f(1,1,0);
    glTranslated(root->next->next->vcoord[0], 
		 root->next->next->vcoord[1], 
		 root->next->next->vcoord[2]);
    glutSolidSphere(0.05,16,16);
    glColor3f(r,g,b);
    glPopMatrix();		
    
    node = list;
    
    do{
      
      glPushMatrix();
      tv = (Vertex *)(node->p);
      glTranslated(tv->vcoord[0], 
		   tv->vcoord[1], 
		   tv->vcoord[2]);
      glutSolidSphere(0.035,16,16);
      glPopMatrix();		      
      node = node->next;
    }while( node != list);
    
    
    glColor3f( 1,1,0);
    node = hlist;
    do{
      
      
      tv1 = HalfEdgeStartVertex( (HalfEdge*) node->p );
      tv2 = HalfEdgeEndVertex( (HalfEdge*) node->p );
      
      glBegin(GL_LINES);
      glVertex3f(tv1->vcoord[0],
		 tv1->vcoord[1],
		 tv1->vcoord[2]);
      glVertex3f(tv2->vcoord[0],
		 tv2->vcoord[1],
		 tv2->vcoord[2]);
      glEnd();
      node = node->next;
    }while( node != hlist );
    glColor3f(r,g,b);
    break;
  } 
}

void MenuFunc( int value ){
  switch( value ){
  case 0:
    display_mode = 0;
    break;
  case 1:
    display_mode = 1;
    break;
  case 2:
    display_mode = 2;
    break;
  }
  glutPostRedisplay();
}

void SetMenu( void ){
  int menu;

  menu =  glutCreateMenu( MenuFunc );
  glutAddMenuEntry("Show faces", 0);
  glutAddMenuEntry("Show edges", 1);
  glutAddMenuEntry("Debug", 2);
  glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
    
/****************************************************************************
   Functions for Display
 ***************************************************************************/

/*--------------------------------------------------------------------------
  Go from the world coordinate system to red robot local coordinate system
  --------------------------------------------------------------------------*/


void setupObjG()
{ 
  GLdouble rotGL[16];
  /* !! apply proper transformations */
  glTranslatef( objGreenTransCoords.x,
		objGreenTransCoords.y,
		objGreenTransCoords.z); 

  tempRotMat = putInMatrix(objGreenRotMat);
  mat2GL( tempRotMat, rotGL );
  glMultMatrixd( (GLdouble *) rotGL );

}

/*-------------------------------------------------------------------------
  Display the floor , which is attached to the current floor
  ---------------------------------------------------------------------------*/
void displayfloor(void)
{
  glPushMatrix();  
  glTranslatef( 0, - 3.5 , 0 );
  glEnable(GL_CULL_FACE);
  drawfloor();
  glDisable(GL_CULL_FACE);
  glPopMatrix();
}



/*--------------------------------------------------------------------------
  Setup Eye, go from the local coordinate system to the world coordinate system
  ---------------------------------------------------------------------------*/
void setupEye(void)
{ 
  GLdouble rot[16];
  

 
  switch(View){
  case VIEW_FROM_SKYCAM:
    tempRotMat = putInMatrix(qrotInv(skyRotMat));
    mat2GL( tempRotMat ,rot);
    glTranslatef( -skyTransCoords.x,
		  -skyTransCoords.y,
		  -skyTransCoords.z); 
    
    glMultMatrixd( (GLdouble *) rot) ;
    break;
    
  default:
      
    glRotatef(90,1,0,0);
    glTranslatef( 0,-10,0); 

    /* in torus case , the camera should translated to ( 0,-50,0) */

    break;
  }


}

/*--------------------------------------------------------------------------
  Display
  Function
  --------------------------------------------------------------------------*/
void displayNextMergedHalfEdge(float r, float g, float b){

  HalfEdge * mate ;
  Vertex *tv1, *tv2;
  if( ! nextMergedHalfEdge ) 
    return;

  if( ! nextMergedHalfEdge->aliveh )
    return;

  mate = HalfEdgeMate( nextMergedHalfEdge );
  tv1 = nextMergedHalfEdge->hvert;
  tv2 = mate->hvert;

  glColor3f( r , g, b );
  
  glLineWidth(3.0);

  glBegin(GL_LINES);
  
  glVertex3f(tv1->vcoord[0],
	     tv1->vcoord[1],
	     tv1->vcoord[2]);
  
  glVertex3f(tv2->vcoord[0],
	     tv2->vcoord[1],
	     tv2->vcoord[2]);
  glEnd();
  glLineWidth(1.0);
  /*
   displayStructure( 0, 0, 1,2, tv1);  
   displayStructure( 1, 0, 0,2, tv2);  
   */
}
void displayEdge(Solid * solid,float r, float g, float b){
  
  
  Edge * te;
  Edge * head = solid->sedges;
  Vertex *tv1, *tv2;
  int i = 0;
  
  if(! head ) return;
  te = head;
  
  
  do{
    if( te->alive ){
      i++;
      if( te->he1 != nextMergedHalfEdge && te->he2 != nextMergedHalfEdge )
	glColor3f( r , g, b );
      else
	glColor3f( 1, 0 , 0 );
	
	tv1 =  te->he1->hvert;
	tv2 =  te->he2->hvert;
	
	glBegin(GL_LINES);
	
	glVertex3f(tv1->vcoord[0],
		   tv1->vcoord[1],
		   tv1->vcoord[2]);
	
	glVertex3f(tv2->vcoord[0],
		   tv2->vcoord[1],
		   tv2->vcoord[2]);
	glEnd();
      }
    te = te->next;
  }while( te != head );
  
  
  
}


void drawFace(Vertex * vert ){
  
  
  static Face * tf = NIL;
  HalfEdge    * he;
  
  if( !vert ){
    tf = NIL;
    return;
  }
  if( !tf ) tf = VertexFirstFace( vert );
  else tf = VertexNextFace( vert, tf );

  glColor3f(0,0.8,0);
  glBegin(GL_TRIANGLES);

  he =  tf->floop->ledges;
  
  do{
    
    glNormal3f(he->hvert->ncoord[0],
	       he->hvert->ncoord[1],
	       he->hvert->ncoord[2]);
    
    glVertex3f(he->hvert->vcoord[0],
	       he->hvert->vcoord[1],
	       he->hvert->vcoord[2]);
    
    he = he->next;
  }while( he != tf->floop->ledges); 
  
   printf("\n");
  glEnd();
 
}

void displayFace( Solid * solid ){
  
  
  Face * tf;
  HalfEdge *he;
  Face * head = solid->sfaces;
  
  
  if(! head ) return;
  tf = head;
  glColor3f(0,0.8,0);
  glBegin(GL_TRIANGLES);

  do{
      he =  tf->floop->ledges;

      do{
	
		
	glNormal3f(he->hvert->ncoord[0],
		   he->hvert->ncoord[1],
		   he->hvert->ncoord[2]);

	glVertex3f(he->hvert->vcoord[0],
		   he->hvert->vcoord[1],
		   he->hvert->vcoord[2]);
	
	
	he = he->next;
      }while( he != tf->floop->ledges); 

      tf = tf->next;
  }while( tf != head );
  
  glEnd();
 
}
void displayVisibleFace( Solid * solid ){
  
  
  Face * tf;
  HalfEdge *he;
  Face * head = solid->sfaces;
  
  
  if(! head ) return;
  tf = head;
  glColor3f(0,0.8,0);
  glPolygonMode( GL_FRONT, GL_LINE );
  glCullFace( GL_BACK );
  glBegin(GL_TRIANGLES);

  do{
    if( tf->alivef ) {
      he =  tf->floop->ledges;

      do{
	
		
	glNormal3f(he->hvert->ncoord[0],
		   he->hvert->ncoord[1],
		   he->hvert->ncoord[2]);

	glVertex3f(he->hvert->vcoord[0],
		   he->hvert->vcoord[1],
		   he->hvert->vcoord[2]);
	
	
	he = he->next;
      }while( he != tf->floop->ledges); 
    }
      tf = tf->next;
  }while( tf != head );
  
  glEnd();

  glPolygonMode( GL_FRONT, GL_FILL );
 
}

void checkManifold( Solid * solid ){
  
  
  Face * tf;
  HalfEdge * he;
  Vertex   * vert;
  Face * head = solid->sfaces;
  
  
  if(! head ) return;
  tf = head;

  do{
    if( tf->alivef ) {
      he =  tf->floop->ledges;

      do{
	vert = HalfEdgeEndVertex( he );
	if( VertexFaceNumber( vert ) < 3 ){
	  fprintf(stderr,"checkManifold::Non manifold\n");
	  return;
	}
	he = he->next;
      }while( he != tf->floop->ledges); 
    }
      tf = tf->next;
  }while( tf != head );
  fprintf(stderr,"checkManifold::manifold\n");
	  
 
}

/*-------------------------------------------------------------------------

  Give halfedge list in merged halfedge list, extend halfedges

--------------------------------------------------------------------------*/
void Recover(){

 Node * node;
 HalfEdge * he;

 if( !merged_halfedge_list ) return;

 node = merged_halfedge_list;
 
 node = node->prev;
 he = (HalfEdge * ) node->p;
 
 HalfEdgeExtend( he );

 ListDeleteNode( &merged_halfedge_list, (void*)he, HALFEDGE);

 checkManifold( solid );

}

/*--------------------------------------------------------------------------
  Display Function !!
  --------------------------------------------------------------------------*/
void display(void)
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  setupEye();   
  
  
  glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
  glLightfv(GL_LIGHT2, GL_POSITION, lightTwoPosition);
  
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  
  /*---------------------------------------------------------------------*/
  


  displayfloor();
  drawprops();
  glPushMatrix();
  
  
  setupObjG();

  switch( display_mode ){
  case 0:
    displayFace( solid );
    break;
  case 2:
    displayVisibleFace( solid );
    break;
   case 1:
    displayEdge( solid, 1,1,1 );
    break;
  }

  if(  display_next_merged_halfedge ){
    displayNextMergedHalfEdge( 1, 0 , 0 );
    if( nextMergedHalfEdge ){
      if( !start_flag )
	drawFace(nextMergedHalfEdge->hvert );
      else
	drawFace(HalfEdgeEndVertex( nextMergedHalfEdge ) );
      
    }
    else
       drawFace(NIL );
  }

  glPopMatrix();
  


  glFlush();
  
  glutSwapBuffers();
}


/*---------------------------------------------------------------------------
  shrink the vector (dx,dy) to (fx,fy) with length 0.1
  ---------------------------------------------------------------------------*/
void shrink(int dx, int dy, double *fx, double *fy)
{
 
  int    ax,ay;
 
  ax = (dx>0)? dx:-dx;
  ay = (dy>0)? dy:-dy;

  if( !ax && !ay ) { 
    *fx = *fy = 0;
    return;
  }
  if( ax > ay ){
    *fx = (dx>0)? 0.1*(float)ax:-0.1*(float)ax;
    *fy = (dy>0)? 0.1*(float)ay:-0.1*(float)ay;
  }
  else{
    *fy = (dy>0)? 0.1*(float)ay:-0.1*(float)ay;
    *fx = (dx>0)? 0.1*(float)ax:-0.1*(float)ax;
  }

}


void OperateRelative2Local(int operation,
			   int dx, 
			   int dy,
			   qrot *objRotMat,
			   coords3 *objTransCoords)
{
  double fx,fy;
  qrot Q; /* new rotation */
  
  
  switch(operation){
  case FORWARD: /* w TRS = W T(RSR')R */
    shrink(dx,dy,&fx,&fy);
    *objTransCoords = coordAdd( *objTransCoords, 
				qrotApply(*objRotMat,
					  setCoords( fx,fy, 0 )));
    break;

  case JUMP: /* w TRS = W T(RSR')R */
    shrink(dx,dy,&fx,&fy);
    *objTransCoords = coordAdd( *objTransCoords, 
				qrotApply(*objRotMat,
					      setCoords( 0.0, 0, -fy )));
    break;



  case TURN: /* wTRQ = wT(RQ) */
    Q = qrotCompose(
		  roty2qrot(2*dx),
		  rotx2qrot(-2*dy)
		  );
    *objRotMat = qrotCompose(*objRotMat,Q);
    break;
  }
}



void OperateRelative2World(int operation,
			   int dx, 
			   int dy,
			   qrot *objRotMat,
			   coords3 *objTransCoords)

{
  double fx,fy;
  qrot Q; /* new rotation */
  
  
  switch(operation){
  case FORWARD: /* w STR = W (ST)R */
    shrink(dx,dy,&fx,&fy);
    *objTransCoords = coordAdd( *objTransCoords,setCoords( fx, fy, 0 )); 
    break;

  case JUMP: /* w STR = W (ST)R */
    shrink(dx,dy,&fx,&fy);
    *objTransCoords = coordAdd( *objTransCoords,setCoords( 0., 0,- fy )); 
    break;


  case TURN: /* wQTR = w(QTQ')(QR) */
    Q = qrotCompose(
		  roty2qrot(2*dx),
		  rotx2qrot(-2*dy)
		  );
    *objRotMat = qrotCompose(Q,*objRotMat);
    *objTransCoords = qrotApply(Q,*objTransCoords);
    break;
  }
}

void OperateRelative2SkyCamera(int operation,
			       int dx, 
			       int dy,
			       qrot newRot,
			       qrot *objRotMat,
			       coords3 *objTransCoords,
			       qrot skyRotMat)

{
  double fx,fy;
  qrot Q; /* new rotation */
  
  
  switch(operation){ /* wTR = NRs'R , NSRs'R = WT0RsSRs' R */
  case FORWARD: 
    shrink(dx,dy,&fx,&fy);
     *objTransCoords = 
       coordAdd(qrotApply( skyRotMat, 
			       setCoords(fx,fy,0) ), *objTransCoords);
    break;
  case JUMP: 
    shrink(dx,dy,&fx,&fy);
     *objTransCoords = 
       coordAdd(qrotApply( skyRotMat, 
			       setCoords(0.0,0,-fy) ), *objTransCoords);
    break;

  case TURN: /* wQTR = w(QTQ')(QR) */
    if( !dx && !dy ) break;
    Q = newRot;
    Q = qrotCompose(qrotCompose( skyRotMat, Q ), qrotInv( skyRotMat ));
    *objRotMat = qrotCompose(Q,*objRotMat);
    break;
  }
}






void OperateSkyCamera(int operation,
			   int dx, 
			   int dy,
			   qrot newRot,
			   qrot *objRotMat,
			   coords3 *objTransCoords
			  )

{
  double fx,fy;
  qrot Q; /* new rotation */
  coords3 S;
  coords3 center;
  
  /* wRTS = NTS */

  switch(operation){
  case FORWARD: 
    shrink(dx,dy,&fx,&fy);
    S = setCoords(-fx,-fy, 0);
   *objTransCoords = coordAdd(S, *objTransCoords );
    
    break;
  case JUMP:  
    shrink(dx,dy,&fx,&fy);
    S = setCoords(0.0,0,fy);
   *objTransCoords = coordAdd(S, *objTransCoords );
    break;
  case TURN: /* wRT = NQ'T */
    Q = qrotInv(newRot);
    *objRotMat = qrotCompose( *objRotMat, Q );
							 
    center = setCoords(0,0,0);
    center = objGreenTransCoords;

    *objTransCoords = 
      coordAdd(*objTransCoords,
	       coordSub(
			qrotApply(qrotInv(Q),
				  qrotApply(qrotInv(*objRotMat),center)),
			qrotApply(qrotInv(*objRotMat),center)));
    
    break;
  }
}



/*---------------------------------------------------------------------------
  Update the matrices for red robot and green robot , and their inverses
  ---------------------------------------------------------------------------*/
void update(int operation, int dx, int dy, qrot newRot)
{
  /* !! */


  switch( Object ){
  case SKYCAM :
    OperateSkyCamera( operation,dx,dy, newRot,
		      &skyRotMat, &skyTransCoords );
    break;
  case OBJECTG:
    switch(Mode ){
    case RELATIVE_TO_LOCAL:
      OperateRelative2Local(operation,dx, dy, 
			    &objGreenRotMat,
			    &objGreenTransCoords);
      break;
    case RELATIVE_TO_WORLD:
      OperateRelative2World(operation,dx, dy, 
			    &objGreenRotMat,
			    &objGreenTransCoords);
      break;
    case RELATIVE_TO_SKYCAM:
      OperateRelative2SkyCamera(operation,dx, dy, newRot,
				&objGreenRotMat,
				&objGreenTransCoords,
				skyRotMat);
    }
    break;
  }
  glutPostRedisplay();
}





/****************************************************************************
  Interface Functions
  **************************************************************************/

/*--------------------------------------------------------------------------
  Middle button down - start forward
  Left   button down - start to turn
  ---------------------------------------------------------------------------*/

int current_x;
int current_y;
int left_button_down = 0;
int right_button_down = 0;

void mouse(int button, int state, int x, int y) 
{   
  

  if (button == GLUT_LEFT_BUTTON) 
    left_button_down = ( state == GLUT_DOWN );
 
  if (button == GLUT_RIGHT_BUTTON)
    right_button_down = ( state == GLUT_DOWN );
  


  if( state != GLUT_DOWN ) return;

  current_x = x;
  current_y = y;

  setWinArc(GW,GH,x,GH-y);
  


    
  if( left_button_down && right_button_down ){
    Operation = JUMP;
    return;
  }


  if ( !left_button_down && button == GLUT_RIGHT_BUTTON ){
    Operation = FORWARD;
    return;
  }

  if( left_button_down && !right_button_down){
    Operation = TURN;
    return;
  }

}

/*----------------------------------------------------------------------------

  Mesh simplification stuff

---------------------------------------------------------------------------*/

double  FaceCost(Face * ){
  
  return (double)(rand()/65535.0);
}




void HeapUpdate(Vertex * root){

  Vertex * tv,*fv;
  HalfEdge *he, *te;
  Node * node, *tn;
  double old_value, new_value;

  /*
   *
   * Construct update set of vertex root, store all the halfedges in hlist
   *
   */

  ListDestruct( &hlist );
  
  fv = VertexFirstVertex( root );
  tv = fv;
  do{
    
    he = VertexFirstOutHalfEdge( tv );
    te = he;
    do{
      
      ListInsertNode( &hlist,(void*)te, HALFEDGE );
      te = VertexNextOutHalfEdge(te);
    }while( te != he ); 
    
    
    he = VertexFirstInHalfEdge( tv );
    te = he;
    do{
      ListInsertNode( &hlist,(void*)te, HALFEDGE );
      te = VertexNextInHalfEdge(te);
    }while( te != he ); 
      

  tv = VertexNextVertex( root, tv);
 }while(tv != fv);


 
 tn = hlist;
 do{
   node = heapNode( tn->p );
   if( node ){
     old_value = node->v;
     new_value = HalfEdgeCost((HalfEdge*)tn->p, FaceCost );
     node->v   = new_value;
     if( old_value < new_value ) heapDownheap(tn->p);
     else                        heapUpheap(tn->p);
   }
   tn = tn->next;
 }while( tn != hlist );

}


HalfEdge * HalfEdgeMergeProcessStepOne(){

Node * tn;
HalfEdge * te;

      if( heapEmpty() ) return NIL;

      /* select min cost halfedge */
       tn = heapSelectMin();
       te = (HalfEdge*)tn->p;
       return te;
}



void HalfEdgeMergeProcessStepTwo( HalfEdge * te ){

Node * tn;
HalfEdge *mate, *henext, *heprev,*matenext, *mateprev;
Vertex * end;
      if( !te ) return;
      /* check if te is mergable first */
      if(! HalfEdgeMergable( te ) ){
	fprintf(stderr,"HalfEdgeMergeProcessStepTwo::halfedge is not mergable\n");
	nextMergedHalfEdge = NIL;
	return;
      }
      
      end = HalfEdgeEndVertex( te );      
      assert( te->aliveh );
   
      /* remove mate of the min cost halfedge */
      
      mate = HalfEdgeMate( te );
      
      tn = heapNode( (void*)mate );
      if( tn ){
	tn->v = -200.0;
	heapUpheap( (void*)mate );
	heapSelectMin();
      }
      
      henext = te->next;
      tn = heapNode( (void*)henext );
      if( tn ){
	tn->v = -200.0;
	heapUpheap( (void*)henext );
	heapSelectMin();
      }
      
      heprev = te->prev;
      tn = heapNode( (void*)heprev );
      if( tn ){
	 tn->v = -200.0;
	 heapUpheap( (void*)heprev );
	 heapSelectMin();
      }
      
      matenext = mate->next;
      tn = heapNode( (void*)matenext );
      if( tn ){
	tn->v = -200.0;
	heapUpheap( (void*)matenext );
	heapSelectMin();
      }
      
      mateprev = mate->prev;
       tn = heapNode( (void*)mateprev );
       if( tn ){
	 tn->v = -200.0;
	 heapUpheap( (void*)mateprev );
	 heapSelectMin();
       }
       
       
       /* merge halfedge with minimum cost */
       
       HalfEdgeMerge( te );
       
       /* update neighborhood of the halfedge removed */
       HeapUpdate( end );
       
       /* check consistency of heap */
       //heapCheck();

       nextMergedHalfEdge = NIL;
       
       /* record the merged halfedge */
       ListInsertNode( &merged_halfedge_list,(void*)te, HALFEDGE );
       
       
}




void HalfEdgeHeapInitialize(Solid * solid){
  
  Edge * edge;
  HalfEdge *he;
  Node * na[65536], *tn;
  int i = 0,n;
  
  /*
   *
   * construct halfedge array na
   *
   */

  
 edge = solid->sedges;
 
 do{
   
   he = edge->he1;
   ListInsertNode( &halfedge_list,(void*)he, HALFEDGE );
   he = edge->he2;
   ListInsertNode( &halfedge_list,(void*)he, HALFEDGE );
   edge = edge->next;

 }while( edge != solid->sedges );
 
 
 tn = halfedge_list;
 do{
   na[++i] = tn;
   tn->v = HalfEdgeCost((HalfEdge*)tn->p, FaceCost );
   tn = tn->next;
 }while( tn != halfedge_list );
 n = i; 
 
 /*
  *
  * construct heap
  *
  */
 
 
 /*
   heapsort(na,n);
   for( i=1;i<=n;i++)
   printf("HeapSort %f\n",na[i]->v);
   */
 
 heapConstruct(na,n);
 heapCheck();
 // heapPrint();
 //father is smaller than both left and right children
     
  
}






/*---------------------------------------------------------------------------
   KeyFunc to update the current state
   --------------------------------------------------------------------------*/

void KeyFunc(unsigned char key, int x, int y){
  
  int posx,posy;
  int i;
  posx = x;
  posy = y;

  switch(key){

  case 'g':
    Object = OBJECTG;
    break;

  case 's':
    Object = SKYCAM;
    break;

  case 'v' :
    View = !View;
    break;

  case 'W':
    Mode = RELATIVE_TO_WORLD;
    break;

  case 'L':
    Mode = RELATIVE_TO_LOCAL;
    break;
  case 'S':
    Mode = RELATIVE_TO_SKYCAM;
    break;
  case '0':
    display_next_merged_halfedge = ! display_next_merged_halfedge;
    break;

  case '1':
    nextMergedHalfEdge =  HalfEdgeMergeProcessStepOne();
    break;
  case '2':
    HalfEdgeMergeProcessStepTwo( nextMergedHalfEdge );
    checkManifold( solid );
    break;
  case '3':
    drawFace(0 );
    start_flag = !start_flag;
    break;
  case 'f':
    drawFace( nextMergedHalfEdge->hvert );
    return;
    break;
  case 'r':
    Recover();
    break;
  case 'p':
    heapPrint();
    break;
  case 'q':
    for( i = 0; i < 50 ; i ++ ){
          nextMergedHalfEdge =  HalfEdgeMergeProcessStepOne();
	  HalfEdgeMergeProcessStepTwo( nextMergedHalfEdge );
	  //checkManifold( solid );
	  printf("current step is %d\n", i );
    }
    break; 
  }
  
  if( Object == SKYCAM )  Coordnate = SKYCAM_COORD;
  else
    
    switch(Mode){
    case RELATIVE_TO_LOCAL:
      switch(Object){
      case OBJECTG: Coordnate = OBJECTG_COORD;
	break;
      }
    case RELATIVE_TO_WORLD:
      Coordnate = WORLD_COORD;
      break;
      
    }
  
  glutPostRedisplay();
}





/****************************************************************************
 Initialization Functions
 ***************************************************************************/
/*--------------------------------------------------------------------------
  Setup GL sate
  ---------------------------------------------------------------------------*/
void setupGLstate()
{

  glFrontFace(GL_CCW);      
  glEnable(GL_DEPTH_TEST);
  glClearColor(0,0,0,0);
  glShadeModel(GL_FLAT);


  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightTwoColor); 
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

  glShadeModel(GL_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

/*----------------------------------------------------------------------------
  Initialize green robot matrix and its inverse
  ---------------------------------------------------------------------------*/

void InitializeobjGMatrix()
{

  objGreenRotMat = qrotIdent();
  objGreenTransCoords = setCoords(0,0,0);

  
}
/*----------------------------------------------------------------------------
  Initialize Sky Camera
  ---------------------------------------------------------------------------*/

void InitializeSkyCamera()
{
  skyRotMat = 
  qrotCompose(
	    roty2qrot(  45.0 ),
	    rotx2qrot( -45.0 ));


  skyTransCoords = setCoords(0,0,10);

  
}

/*----------------------------------------------------------------------------
  Setup Global Environment state
  ---------------------------------------------------------------------------*/

void InitializeEnvState()
{
  
  InitializeobjGMatrix();
  InitializeSkyCamera();
}


void InitializeGlobal(){



}


/****************************************************************************
  Resize the window
  **************************************************************************/
void
reshape(int w, int h)
{
  GW = w;
  GH = h;
  AR = (float)(w)/(float)(h);
  glViewport(0, 0, w, h);               /* Set Viewport */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  
  // magic imaging commands
  gluPerspective( 40.0, /* field of view in degrees */
		  AR, /* aspect ratio */
		  0.1, /* Z near */  
		  500.0 /* Z far */);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
  
  glutPostRedisplay();
  
}


/*--------------------------------------------------------------------------
    Called when a the mouse is moved in the window, while a button
    is pressed.
    -------------------------------------------------------------------------*/

void mouseMove(int x, int y)
{ 
  int dx = x - current_x;
  int dy = y - current_y;
  qrot newRot;
  
  newRot = winArcTform(x,GH-y);

  current_x = x;
  current_y = y;

  setWinArc(GW,GH,x,GH-y);
  
  switch( Object ){
  case OBJECTR: 
    Object = OBJECTR;
    update( Operation, dx,-dy,newRot );
    break;
  case OBJECTG:
    Object = OBJECTG;
    update( Operation, dx,-dy,newRot );   
    break;
    
  case SKYCAM:
    update(Operation, dx , -dy, newRot );
    break;
  }

  glutPostRedisplay();
}






void SolidScale( Solid * solid , double factor ){

  Vertex * tv, * head;
  
  head = solid->sverts;
  if( !head ) return;
  tv = head;
  do{

    tv->vcoord[0] *= factor;
    tv->vcoord[1] *= factor;
    tv->vcoord[2] *= factor;

    tv = tv->next;

  }while( tv != head );

}


/*****************************************************************************
 Main Function
 ****************************************************************************/

int main(int argc, char **argv) 
{  
  double factor;

  /* glut stuff */
  SolidConstructNoff( &solid,argv[1] );
  factor = atof( argv[2] );

  /*
  if( !strncmp( argv[1], "../meshes/holes", 15 ) ){
    SolidScale( solid2 , 1.0/15.0 );
    SolidScale( solid  , 1.0/15.0 );
    model = TORUS;
  }
  */
  
  SolidScale( solid , factor );
  HalfEdgeHeapInitialize( solid );


  
  glutInit(&argc, argv);                /* Initialize GLUT */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize(400, 400);
  glutCreateWindow("my window");        /* Create window with given title */
  glViewport(0,0,400,400 );

  glutDisplayFunc(display);             /* Set-up callback functions */
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMove);
  glutKeyboardFunc(KeyFunc);

  setupGLstate();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  InitializeEnvState();
  InitializeGlobal();

  SetMenu();
  glutMainLoop();                       /* Start GLUT event-processing loop */

  ListDestruct( & merged_halfedge_list );
  return 0;


}
