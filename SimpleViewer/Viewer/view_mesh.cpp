#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<GL/glut.h>
#include "viewer/arcball.h"                           /*  Arc Ball  Interface         */
#include "Mesh/TriangleSoup.h"
#include "bmp/RgbImage.h"

//compiling command
//g++ -lGL -lglut -lGLU -IMeshLib view_mesh.C -o view_mesh

using namespace MeshLib::TriangleSoup;

enum mesh_type{OBJ,M};


CTriangleMesh mesh;
CObject       object;

mesh_type current_mesh_type;

/* window width and height */
int win_width, win_height;
int gButton;
int startx,starty;
int shadeFlag   = 0 ;

/* rotation quaternion and translation vector for the object */
MeshLib::CQrot       ObjRot(0,0,1,0);
MeshLib::CPoint      ObjTrans(0,0,0);
/* arcball object */
MeshLib::CArcball arcball;

/* geometry flag */
int geometryFlag = 0;

/* texture flag */
int textureFlag =  2; 
/* texture id and image */
GLuint texName;
RgbImage image;


/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void)
{
    double rot[16];

    glTranslated( ObjTrans[0], ObjTrans[1], ObjTrans[2]);
    ObjRot.convert( rot );
    glMultMatrixd(( GLdouble *)  rot );
}

/*! the eye is always fixed at world z = +5 */

void setupEye(void){
  glLoadIdentity();
  gluLookAt( 0,0, 5,0,0,0,0,1,0);
}

/*! setup light */
void setupLight()
{
	//MeshLib::CPoint position(0,0,-1);
	MeshLib::CPoint position(0,0,1);
	GLfloat lightOnePosition[4]={position[0], position[1], position[2], 0};
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
	MeshLib::CPoint position2(0,0,-1);
	GLfloat lightTwoPosition[4]={position2[0], position2[1], position2[2], 0};
	glLightfv(GL_LIGHT2, GL_POSITION, lightTwoPosition);
}

/*! draw mesh */
void draw_mesh()
{
	  glBindTexture(GL_TEXTURE_2D, texName);

	//glColor3f( 1.0, 1.0, 1.0 );
	//glDisable( GL_LIGHTING );
	for( size_t i = 0; i < mesh.faces().size(); i ++ )
	{
		CFace * pF = mesh.faces()[i];
  		glBegin(GL_TRIANGLES);
		if( shadeFlag == 0 )
		{
			glNormal3f( pF->m_normal[0], pF->m_normal[1], pF->m_normal[2] );
			//glNormal3f( -pF->m_normal[0], -pF->m_normal[1], -pF->m_normal[2] );
			//std::cout << pF->m_normal[0] << " " << pF->m_normal[1] << " " << pF->m_normal[2] << std::endl;
		}
		for( int j = 0; j < 3; j ++ )
		{
			CVertex * pV = pF->m_v[j];
			if( shadeFlag == 1 )
				glNormal3f( pV->m_normal[0], pV->m_normal[1], pV->m_normal[2] );

	
			if( mesh.m_has_uv )
			{
				MeshLib::CPoint2 uv = pV->m_uv;
				glTexCoord2d( uv[0], uv[1] );
			}

			MeshLib::CPoint skin_rgb( 235/255.0,180/255.0,173/255.0);

			if( mesh.m_has_rgb )
				glColor3f( pV->m_rgb[0], pV->m_rgb[1], pV->m_rgb[2] );
			else
				//glColor3f( 1,1,1 );
				glColor3f( skin_rgb[0], skin_rgb[1],skin_rgb[2] );

			if( geometryFlag && mesh.m_has_uv )
			{
				glVertex3d( pV->m_uv[0], pV->m_uv[1] , 0);
			}
			else
			{
				glVertex3d( pV->m_point[0], pV->m_point[1], pV->m_point[2] );
			}
		}
		glEnd();
	}

}



/*! draw object */
void draw_object()
{
	  glBindTexture(GL_TEXTURE_2D, texName);

	for( size_t i = 0; i < object.faces().size(); i ++ )
	{
		CObjFace & pF = object.faces()[i];
  		glBegin(GL_TRIANGLES);
		if( shadeFlag == 0 )
		{
			glNormal3f( pF.m_normal[0], pF.m_normal[1], pF.m_normal[2] );
		}
		for( int j = 0; j < 3; j ++ )
		{
			CObjVertex & pV = pF.m_v[j];
			if( shadeFlag == 1 )
			{
				MeshLib::CPoint n = object.normals()[ pV.m_inl ];
				glNormal3f( n[0], n[1], n[2] );
			}
	
			if( object.m_has_uv )
			{
				MeshLib::CPoint2 uv = object.uvs()[pV.m_iuv];
				glTexCoord2d( uv[0], uv[1] );
			}

			MeshLib::CPoint skin_rgb( 235/255.0,180/255.0,173/255.0);
			glColor3f( skin_rgb[0], skin_rgb[1],skin_rgb[2] );

			if( geometryFlag && object.m_has_uv )
			{
				MeshLib::CPoint2 uv = object.uvs()[pV.m_iuv];
				glVertex3d( uv[0], uv[1] , 0);
			}
			else
			{
				MeshLib::CPoint p = object.points()[pV.m_ipt];
				glVertex3d( p[0], p[1], p[2] );
			}
		}
		glEnd();
	}

}


/*! display call back function
*/
void display()
{
	/* clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLight();
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();
  	/* draw the mesh */
	switch( current_mesh_type )
	{
	case OBJ:
		draw_object();
		break;
	case M:
		draw_mesh();
		break;
	}
	glPopMatrix();
	glutSwapBuffers();
}

/*! Called when a "resize" event is received by the window. */

void reshape(int w, int h)
{
  float ar;

  win_width=w;
  win_height=h;

  ar = (float)(w)/h;
  glViewport(0, 0, w, h);               /* Set Viewport */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // magic imageing commands
  gluPerspective( 40.0, /* field of view in degrees */
		  ar, /* aspect ratio */
		  1.0, /* Z near */
		  100.0 /* Z far */);

  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();
}

/*! helper function to remind the user about commands, hot keys */
void help()
{
  printf("w - Wireframe Display\n");
  
  printf("f  -  Flat Shading \n");
  printf("s  -  Smooth Shading\n");
  printf("t  -  Texture Mapping\n");
  printf("r  -  Reset the view\n" );
  printf("g  -  Toggle Geometry and UV view\n");

  printf("o  -  Save frame buffer to snap_n.bmp\n");
  printf("? -  Help Information\n");
  printf("esc - quit\n");
}

/*! Keyboard call back function */

void keyBoard(unsigned char key, int x, int y)
{

	switch( key )
	{
	case 'f':
		//Flat Shading
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shadeFlag = 0;
		break;
	case 's':
    		//Smooth Shading
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shadeFlag = 1;
		break;
	case 'w':
	  	//Wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

  case 't':
    textureFlag = (textureFlag +1 )%3;
    switch( textureFlag )
    {
    case 0:
        glDisable(GL_TEXTURE_2D);
        break;
    case 1:
        glEnable(GL_TEXTURE_2D);
		    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        break;
    case 2:
       glEnable(GL_TEXTURE_2D);
		    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
       break;
    }
    break;

  case 'g':
	  geometryFlag = (geometryFlag + 1)%2;
	  break;

  case 'r':
	  	ObjRot = MeshLib::CQrot(0,0,1,0);
		ObjTrans = MeshLib::CPoint(0,0,0);
	  break;

  case '?':
    help();
    break;



  case 27:
		exit(0);
		break;
	}
  glutPostRedisplay();
}

/*! setup GL states */
void setupGLstate(){

  GLfloat lightOneColor[] = {1, 1, 1, 1};
  GLfloat globalAmb[] = {.1, .1, .1, 1};
  GLfloat lightOnePosition[] = {.0,  .0, 1, 0.0};

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  //glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  //glClearColor(0,0,0,0);
  glClearColor(1.0,1.0,1.0,1.0);
  glShadeModel(GL_SMOOTH);


  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightOneColor);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  //glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);

  glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! mouse click call back function */
void  mouseClick(int button , int state, int x, int y){


  /* set up an arcball around the Eye's center
	  switch y coordinates to right handed system  */

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      gButton = GLUT_LEFT_BUTTON;
	  arcball = MeshLib::CArcball( win_width, win_height,  x-win_width/2,  win_height-y-win_height/2);
  }

  if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
      startx = x;
      starty = y;
      gButton = GLUT_MIDDLE_BUTTON;
   }

  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
      startx = x;
      starty = y;
      gButton = GLUT_RIGHT_BUTTON;
   }
  return ;
}

/*! mouse motion call back function */

void mouseMove(int x, int y)
{
	MeshLib::CPoint trans;
	MeshLib::CQrot       rot;

  /* rotation, call arcball */
  if (gButton == GLUT_LEFT_BUTTON )
  {
      rot = arcball.update( x-win_width/2, win_height-y-win_height/2);
      ObjRot =  rot * ObjRot;
      glutPostRedisplay();
  }

  /*xy translation */
  if (gButton == GLUT_MIDDLE_BUTTON)
  {
	  double scale = 10./win_height;
      trans =  MeshLib::CPoint(scale*(x-startx), scale*(starty-y), 0  );
	    startx = x;
	    starty = y;
      ObjTrans = ObjTrans + trans;
      glutPostRedisplay();
  }

  /* zoom in and out */
  if (gButton == GLUT_RIGHT_BUTTON ) {
      double scale = 10./win_height;
      trans =  MeshLib::CPoint(0,0, scale*(starty-y)   );
	    startx = x;
	    starty = y;
      ObjTrans = ObjTrans+trans;
      glutPostRedisplay();
  }

}


void normalize( CTriangleMesh & mesh )
{
	MeshLib::CPoint center(0,0,0);
	for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		center += pV->m_point;
	}
	center /= (double)(mesh.vertices().size());

	for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		pV->m_point -= center;
	}

	double len = -1;
	for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		for( int j = 0; j < 3; j ++ )
		{
			len = (len > fabs(pV->m_point[j]))? len: fabs(pV->m_point[j]);
		}
	}

	for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		pV->m_point /= len;
	}

}



void normalize( CObject & object )
{
	MeshLib::CPoint center(0,0,0);
	for( size_t i = 0; i < object.points().size(); i ++ )
	{
		center += object.points()[i];
	}
	center /= (double)(object.points().size());

	for( size_t i = 0; i < object.points().size(); i ++ )
	{
		object.points()[i] -= center;
	}

	double len = -1;

	for( size_t i = 0; i < object.points().size(); i ++ )
	{
		MeshLib::CPoint p = object.points()[i];
		for( int j = 0; j < 3; j ++ )
		{
			len = (len > p[j])? len: fabs(p[j]);
		}
	}

	for( size_t i = 0; i < object.points().size(); i ++ )
	{
		object.points()[i] /= len;
	}

}


void compute_normal( CTriangleMesh & mesh )
{
	for( size_t i = 0; i < mesh.faces().size(); i ++ )
	{
		CFace * pF = mesh.faces()[i];
		MeshLib::CPoint p[3];
		for( int j = 0; j < 3; j ++ )
		{
			p[j] = pF->m_v[j]->m_point;
		}
		MeshLib::CPoint nor = (p[1]-p[0])^(p[2]-p[0]);
		pF->m_area = nor.norm();

		nor /= nor.norm();
		pF->m_normal = nor;
	}

	for( size_t i = 0; i < mesh.faces().size(); i ++ )
	{
		CFace * pF = mesh.faces()[i];
		MeshLib::CPoint p[3];
		for( int j = 0; j < 3; j ++ )
		{
			p[j] = pF->m_v[j]->m_point;
		}
		MeshLib::CPoint nor = (p[1]-p[0])^(p[2]-p[0]);
		nor /= nor.norm();
		pF->m_normal = nor;

		for( int j = 0; j < 3; j ++ )
		{
			pF->m_v[j]->m_normal += pF->m_normal * pF->m_area;
		}
	}

	for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		pV->m_normal /= pV->m_normal.norm();
	}

    for( size_t i = 0; i < mesh.vertices().size(); i ++ )
	{
		CVertex * pV = mesh.vertices()[i];
		pV->m_area = 0;
	}

    //compute the vertex area
	for( size_t i = 0; i < mesh.faces().size(); i ++ )
	{
		CFace * pF = mesh.faces()[i];
		for( int j = 0; j < 3; j ++ )
		{
			pF->m_v[j]->m_area += pF->m_area/3.0;
		}
	}
}


void compute_normal( CObject & object )
{
	for( size_t i = 0; i < object.faces().size(); i ++ )
	{
		CObjFace * pF = & object.faces()[i];

		MeshLib::CPoint p[3];
		for( int j = 0; j < 3; j ++ )
		{
			p[j] = object.points()[pF->m_v[j].m_ipt];
		}

		MeshLib::CPoint nor = (p[1]-p[0])^(p[2]-p[0]);
		pF->m_area = nor.norm();

		nor /= nor.norm();
		pF->m_normal = nor;
	}

	if( object.m_has_normal ) return;

	object.normals().resize( object.points().size() );

	for( size_t i = 0; i < object.faces().size(); i ++ )
	{
		CObjFace * pF = &object.faces()[i];

		for( int j = 0; j < 3; j ++ )
		{
			pF->m_v[j].m_inl = pF->m_v[j].m_ipt;
			object.normals()[pF->m_v[j].m_inl] += pF->m_normal * pF->m_area;
		}
	}

	for( size_t i = 0; i < object.normals().size(); i ++ )
	{
		object.normals()[i] /= object.normals()[i].norm();
	}

}

/*! initialize bitmap image texture */

void initialize_bmp_texture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR);

	int ImageWidth  = image.GetNumCols();
	int ImageHeight = image.GetNumRows();
	GLubyte * ptr   = (GLubyte * )image.ImageData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
				        ImageWidth,
						ImageHeight, 
				        0, 
				        GL_RGB, 
				        GL_UNSIGNED_BYTE,
						ptr);

    if(textureFlag == 1)
		  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    else if(textureFlag == 2)
		  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}


/*! main function for viewer
*/
int main( int argc, char * argv[] )
{
	std::string name( argv[1] );

	unsigned pos = name.find_last_of(".");
	std::string type  = name.substr( pos +1 );

	if( type == "obj" )
	{
		object.read( argv[1] );
		normalize( object );
		compute_normal( object );
		current_mesh_type = OBJ;

	}
	else if( type == "m" )
	{
		mesh.read_m( argv[1] );
		normalize( mesh );
		compute_normal( mesh );
		current_mesh_type = M;
	}

	textureFlag = 0; 

	if( argc > 2 )
	{
		image.LoadBmpFile( argv[2] );
	}

	/* glut stuff */
	glutInit(&argc, argv);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(600, 600);
	glutCreateWindow("Mesh Viewer");        /* Create window with given title */
	glViewport(0,0,800,800 );

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);
	setupGLstate();

	if( argc > 2 )
	{
		initialize_bmp_texture();
		textureFlag = 2; 
	}
	glutMainLoop();                       /* Start GLUT event-processing loop */

	return 0;
}


