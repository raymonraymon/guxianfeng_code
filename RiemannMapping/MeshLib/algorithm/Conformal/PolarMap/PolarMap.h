/*!
*      \file PolarMap.h
*      \brief Algorithm for computing exponential map \f$ z\to e^{z}\f$
*	   \author David Gu
*      \date Documented 10/12/2010
*
*		Computing the exponential map 
*       \f[
*			f(z) = e^{z}
*		\f]
*
*/

/********************************************************************************************************************************
*
*      Polar Map  Class
*
*       Copyright (c) Stony Brook University
*
*    Purpose:
*
*       Integration Class
* 
*       David Gu June 27, 2008,  gu@cs.stonybrook.edu
*
*
*      Input:
*         
*           Close Mesh, Open Mesh with UV
*
*      Output:
*
*           Exponential Mapping of the mesh
*
*********************************************************************************************************************************/

/*---------------------------------------------------------------------------------------------------------------------------------
#include "PolarMap/PolarMap.h"

using namespace MeshLib;



int main( int argc, char * argv[] )
{
	CMesh mesh;
	mesh.read_m( argv[1] );

	CMesh open_mesh;
	open_mesh.read_m( argv[2] );

	CPolarMap map( &mesh, &open_mesh );
	map._exponential_map();

	mesh.write_m( argv[3] );

	return 0;
}
----------------------------------------------------------------------------------------------------------------------------------*/

#ifndef _POLAR_MAP_H_
#define _POLAR_MAP_H_

#ifndef PI
#define PI 3.14159265358979323846
#endif

#include  <math.h>
#include <queue>
#include <list>
#include <vector>
#include "PolarMapMesh.h"

namespace MeshLib
{
/*! \brief CPolarMap class
*
*	Algorithm for computing complex exponential map \f$ z\to e^{z} \f$.
*/
class CPolarMap
{
public:
	/*! CPolarMap constructor
	*
	*  \param pClosedMesh the output closed mesh
	*  \param pOpenMesh   the input open mesh, with vertex uv trait
	*/
	CPolarMap( CPMMesh * pClosedMesh, CPMMesh * pOpenMesh );
	/*! CPolarMap destructor */
	~CPolarMap();
	/*! Compute the exponential map*/
	void _exponential_map();

private:
	
	/*! the output closed mesh */
	CPMMesh * m_pClosedMesh;
	/*! the input open mesh, with vertex uv trait */
	CPMMesh * m_pOpenMesh;

};

};

#endif