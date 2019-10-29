/*!
*      \file DomainCohomology.h
*      \brief Algorithm for Computing Cohomology Group Basis for Mulply Connected Domains
*	   \author David Gu
*      \date Document 10/11/2010
*
*		Computing Cohomology Group Basis for Mulply Connected Domains
*
*/

#ifndef _DOMAIN_COHOMOLOGY_H_
#define _DOMAIN_COHOMOLOGY_H_

#include "CohomologyMesh.h"

namespace MeshLib
{
	/*! \brief CDomainCohomology class
	*  
	*  Algorithm for computing Cohomology Group Basis for Mulply Connected Domains
	*/
  class CDomainCohomology
  {
  public:
	  /*! CDomainCohomology constructor
	  *
	  *	\param pMesh input closed mesh
	  * \param pWMesh sliced open mesh
	  */
    CDomainCohomology( CCHMesh * pMesh, CCHMesh * pWMesh );
	/*! CHarmonicClosedForm destructor
	*/
    ~CDomainCohomology();
	
	/*!	Compute Cohomology Group Basis for Mulply Connected Domains
	*/
    void calculate_closed_form();

  protected:
	  /*! Input closed mesh */
    CCHMesh * m_pMesh;
	/*! inpute mesh, sliced open along the shortest paths connecting two boundary loops */
    CCHMesh * m_pWMesh;

	/*! Compute the closed 1-form*/
	void _closed_1_form();
	/*! Integrate the closed form on the mesh */
	void _integrate();
	
	//label the boundary loop Id on the closed mesh
	/*! label the boundary loop Id on the closed mesh */
	void _label_closed_mesh_boundary_loop_id();
	//locate four corner vertices
	/*! locate four corner vertices 
	*
	*	\param cut_verts store the four corner vertices in the vector cut_verts
	*/
	void _locate_four_corners(  std::vector<CCHVertex*> & cut_verts );
	
  };
}
#endif _DOMAIN_COHOMOLOGY_H_