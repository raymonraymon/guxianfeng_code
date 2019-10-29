/*!
*      \file BaseHarmonicExactForm.h
*      \brief Algorithm for computing exact harmonic forms
*	   \author David Gu
*      \date Document 10/11/2010
*
*		Compute harmonic exact 1-forms for genus zero surfaces with multiple boundary components.
*
*/

/********************************************************************************************************************************
*
*      Harmonic Exact Form Class
*
*       Copyright (c) Stony Brook University
*
*    Purpose:
*
*       Harmonic Exact Form
* 
*       David Gu June 27, 2008,  gu@cs.stonybrook.edu
*
*
*      Input:
*         
*           Multiple Connected Annulus
*
*      Output:
*
*           Exact Harmonic 1-forms, which is -1 on one inner boundary, and 0 on all the other boundaries. The vertex uv is also set
*
*********************************************************************************************************************************/

/*---------------------------------------------------------------------------------------------------------------------------------
#include "ExactForm/HarmonicExactForm.h"
int main( int argc, char * argv[] )
{

	if( strcmp( argv[1], "-exact_form" ) == 0 )
	{
		CHarmonicMesh hm;
		he.read_m( argv[2] );

		CHarmonicExactForm exact_form( & hm );
		exact_form.calculate_harmonic_exact_form( argv[3] );
		return 0;
	}
}

}
----------------------------------------------------------------------------------------------------------------------------------*/

#ifndef _SLIT_MAP_BASE_HARMONIC_EXACT_FORM_H_
#define _SLIT_MAP_BASE_HARMONIC_EXACT_FORM_H_

#include  <math.h>
#include <queue>
#include <Eigen/Sparse>
#include "HarmonicMesh.h"
#include "Structure/Structure.h"

namespace MeshLib
{
/*! \brief CBaseHarmonicExactForm class
*
*	Compute exact harmonic 1-forms for genus zero surfaces with multiple boundary components
*/
  class CBaseHarmonicExactForm
  {
  public:
	  /*!	CBaseHarmonicExactForm constructor
	  *     \param pMesh input mesh
	  */
    CBaseHarmonicExactForm( CHarmonicMesh * pMesh );
	/*!	CBaseHarmonicExactForm destructor
	*/
    ~CBaseHarmonicExactForm();
	/*!	Calculate harmonic exact forms
	*   \param prefix the prefix of output mesh file
	*/
    void calculate_harmonic_exact_form( const char * prefix);

  protected:
	  /*! the input mesh */
    CHarmonicMesh * m_pMesh;
		/*! the boundary of the input mesh */
	CHarmonicMesh::HBoundary m_boundary;
	
	/*! Compute harmonic exact form, the harmonic function equals to 1 on pL, and zero on other boundary components.
	 * \param pL the boundary loop, on which the function equals to one.
	 */
	void _harmonic_exact_form( CHarmonicMesh::CHLoop * pL );
	/*!
	 *	Compute the angle structure
	 */
	virtual void _angle_structure()=0;

	/*! number of interior vertices */
	int  m_interior_vertices;
	/*! number of boundary vertices */
	int  m_boundary_vertices;
	
  };
}
#endif _SLIT_MAP_BASE_HARMONIC_EXACT_FORM_H_