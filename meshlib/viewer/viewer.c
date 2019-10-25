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



GLfloat green_mat[] = {1.f, .5f, 0.f, .5f};

int   display_mode = 0;

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




void MenuFunc( int value ){
  switch( value ){
  case 0:
    display_mode = 0;
    break;
  case 1:
    display_mode = 1;
    break;
  case 5:
    break;


  }
  glutPostRedisplay();
}

void SetMenu( void ){
  int menu;

  menu =  glutCreateMenu( MenuFunc );
  glutAddMenuEntry("Show faces", 0);
  glutAddMenuEntry("Show edges", 1);
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

void displayEdge(Solid * solid,float r, float g, float b){
  
  
  Edge * te;
  Edge * head = solid->sedges;
  Vertex *tv1, *tv2;
  int i = 0;
  
  if(! head ) return;
  te = head;
  
  
  do{
    if( te->alive >0 ){
      i++;
      glColor3f( r , g, b );

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
    else{
      if( te->alive < 0 ){
	glColor3f( 1,0,0);
	
	
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
    }
    
    te = te->next;
  }while( te != head );
  
  
  
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
  case 1:
    displayEdge( solid , 0,0, 1 );
    break;
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



/*---------------------------------------------------------------------------
   KeyFunc to update the current state
   --------------------------------------------------------------------------*/

void KeyFunc(unsigned char key, int x, int y){
  
  int posx,posy;

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

  return 0;


}







