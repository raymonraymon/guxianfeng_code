/*! \file SlitMap.h
*  \brief Algorithm for computing slit mapping
*  \author David Gu
*  \date Documented on 10/12/2010
*
*	Compute slit maps
*	
*	The inputs are holomorphic basis for the mesh \f$\{\omega_1,\omega_2,\cdots, \omega_{n}\}\f$.
*	Suppose the boundary components are \f$\{\gamma_0,\gamma_1,\gamma_2,\cdots, \gamma_n\}\f$.
*	Find a new holomorphic 1-form \f$\omega = \sum_k \lambda_k \omega_k\f$, such that
*  \f[
*		Img \int_{\gamma_0} \omega = 2\pi, 		Img \int_{\gamma_k} \omega = -2\pi,	Img \int_{\gamma_i} \omega = 0.
*
*  \f]
*/
/********************************************************************************************************************************
*
*      Harmonic Closed Form Class
*
*       Copyright (c) Stony Brook University
*
*    Purpose:
*
*       Compute the holomorphic 1-form for a slit map
* 
*       David Gu June 27, 2008,  gu@cs.stonybrook.edu
*
*
*      Input:
*         
*           basis of holomorphic 1-forms, boundary loop ids, inner and outer boundaries
*
*      Output:
*
*           holomorphic 1-form, imaginary part of the integration are (+ - 2PI )
*
*********************************************************************************************************************************/

/*---------------------------------------------------------------------------------------------------------------------------------
#include <math.h>
#include "mesh/mesh.h"
#include "SlitMap/SlitMap.h"

using namespace MeshLib;

int main( int argc, char * argv[] )
{
   if( strcmp( argv[1], "-slit_map" ) == 0 )
  {

	std::vector<CSMMesh*> meshes;

	for( int i = 2; i < argc-3; i ++ )
	{
		CSMMesh * pMesh = new CSMMesh;
		assert( pMesh );
		pMesh->read_m( argv[i] );
		meshes.push_back( pMesh );
	}

	CSlitMap map( meshes );
	
	int c1 = atoi( argv[argc-3] );
	int c2 = atoi( argv[argc-2] );

	map._slit_map( c1,c2 );

	meshes[0]->write_m( argv[argc-1] );

	for( size_t i = 0; i < meshes.size(); i ++ )
	{
		delete meshes[i];
	}
	return 0;
  } 
}
----------------------------------------------------------------------------------------------------------------------------------*/

#ifndef _SLIT_MAP_H_
#define _SLIT_MAP_H_

#include "SlitMapMesh.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>

namespace MeshLib
{
/*! \brief CSlitMap class
*	
*	Algorithm for computing slit maps
*/
  class CSlitMap
  {
  public:
	 /*! CSlitMap constructor
	 * 
	 * \param meshes the basis of holomorphic 1-forms
	 */
	 CSlitMap( std::vector<CSMMesh*> & meshes );
	 /*! CSlitMap destructor */
     ~CSlitMap();
	/*! Compute the slit map, such that loop c1 becomes the exterior circle, loop c2 becomes the interior circle
	* \param c1 loop id , which becomes the exterior circle
	* \param c2 loop id , which becomes the interior circle
	* other boundary loops become concentric circular slits
	*/
    void _slit_map(int c1 ,int c2);

  protected:
	  /*! holomorphic 1-form basis */
	  std::vector<CSMMesh*> & m_meshes;
	  /*! base mesh */
	  CSMMesh * m_pMesh;
	  /*! integrate a holomorphic 1-form along a boundary loop pL
	   * \param pMesh holomorphic 1-form
	   * \param pL    the boundary loop
	   * \return the integration of the holomorphic 1-form along the boundary loop
	   */
	  double _integrate( CSMMesh * pMesh, CSMMesh::CLoop * pL );
	  /*! the boundary of the input mesh */
	  CSMMesh::CBoundary m_boundary;
	  

  };
}
#endif _SLIT_MAP_H_