/*! \file Integration.h
*   \brief Algorithm for integrating holomorphic 1-forms on a mesh
*	\author David Gu
*   \date Documented on 10/12/2010
*/

/********************************************************************************************************************************
*
*      Integration  Class
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
*           Holomorphic 1-form mesh, and a foundamental domain mesh ( a topological disk )
*
*      Output:
*
*           Integrate the holomorphic 1-form on the fundamental domain, to compute the vertex uv coordinates
*
*********************************************************************************************************************************/


/*---------------------------------------------------------------------------------------------------------------------------------
#include <math.h>
#include "Integration/Integration.h"


using namespace MeshLib;

int main( int argc, char * argv[] )
{
	CIMesh holo_mesh;
	CIMesh fund_mesh;

	holo_mesh.read_m( argv[1] );
	fund_mesh.read_m( argv[2] );

	CIntegration integrator( & holo_mesh, & fund_mesh );
	integrator._integrate();

	fund_mesh.write_m( argv[3] );

	return 0;
}
----------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _INTEGRATION_H_
#define _INTEGRATION_H_

#include <queue>
#include "IntegrationMesh.h"

namespace MeshLib
{
/*! \brief CIntegration class
*
*	Integrates a holomorphic 1-form on a mesh, and get he uv coordiantes for each vertex
*/ 
class CIntegration
{
public:
	/*! CIntegration constructor
	* \param pForm input holomorphic 1-form 
	* \param pDomain output the integration results are stored in the vertex uv field
	*/
	CIntegration( CIMesh * pForm, CIMesh * pDomain );
	/*! CIntegration destructor */
	~CIntegration();
	/*! Integrate holomorphic 1-form on the mesh */
	void _integrate();

private:
	/*! Holomorphic 1-form */
	CIMesh * m_pForm;
	/*! The integration domain mesh, the integration results are stored in the vertex uv field */
	CIMesh * m_pDomain;

};

};

#endif