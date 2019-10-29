/*!
*      \file Puncture.h
*      \brief Algorithm for punching a hole in the center of the mesh, and the reverse operation: filling the hole.
*	   \author David Gu
*      \date Documented on 10/12/2010
*
*/

/*******************************************************************************
*      Puncture Hole on a mesh/ fill a hole of the mesh
*
*       Copyright (c) Stony Brook University
*
*    Purpose:
*
*       Locate the vertex in the center, puncture an adjacent face / locate the smallest boundary, fill it with a face
* 
*       David Gu June 27, 2008, gu@cs.stonybrook.edu
*
*	Input
*       A mesh/ A mesh with a hole with 3 vertices
*	Output
*       The mesh with a face punctured, the face is adjacent to the center vertex / Fill the hole
*
*******************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------------------

#include "puncture/puncture.h"

using namespace MeshLib;

int main( int argc, char * argv[] )
{
  if( strcmp( argv[1], "-puncture" ) == 0 )
  {

	CPMesh cmesh;
	cPmesh.read_m( argv[2] );

	CPuncture cut( & cmesh );
	cut._puncture();
	
	cmesh.write_m( argv[3] );
	return 0;
  }

  if( strcmp( argv[1], "-fill_hole" ) == 0 )
  {

	CPMesh cmesh;
	cmesh.read_m( argv[2] );

	CPuncture cut( & cmesh );
	cut._fill_hole();
	
	cmesh.write_m( argv[3] );
	return 0;
  }

}

--------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PUNCTURE_H_
#define _PUNCTURE_H_

#include <queue>
#include "PunctureMesh.h"

namespace MeshLib
{
/*! \brief CPuncture class
*
*	Algorithm for punching a hole in the center of the mesh
*   and filling the hole. This is used to compute the Riemann mapping
*   using slitMap.
*/
  class CPuncture
  {
  public:
	  /*! CPuncture class constructor
	  * \param pMesh the input mesh
	  */
    CPuncture( CPMesh * pMesh );
	/*! CPuncture class destructor */
    ~CPuncture();

	/*! puncture a hole in the center of the mesh */
	void _puncture();
	/*! fill the holed punctured */
	void _fill_hole();
  
  protected:
	  /*! input mesh */
    CPMesh * m_pMesh;
	/*! the boundary of the input mesh */
	CPMesh::CBoundary m_boundary;

  };
}
#endif