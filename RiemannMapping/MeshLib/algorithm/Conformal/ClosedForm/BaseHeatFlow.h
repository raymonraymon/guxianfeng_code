/*!
*      \file BaseHeatFlow.h
*      \brief Algorithm for base harmonic forms (for closed surfaces)
*	   \author David Gu
*      \date Document 12/26/2010
*
*		Computing harmonic forms (for closed surfaces)
*
*/
#ifndef _BASE_HEAT_FLOW_H_
#define _BASE_HEAT_FLOW_H_

#include "HarmonicClosedFormMesh.h"
#include "Structure/Structure.h"
#include <Eigen/Sparse>
//#include "LinearAlgebra/SparseMatrix.h"

namespace MeshLib
{
	/*! \brief CBaseHeatFlow class
	*  
	*  Algorithm for computing harmonic forms
	*/

  class CBaseHeatFlow
  {
  public:
	  /*! CBaseHeatFlow constructor
	  *
	  *	\param pMesh input closed mesh
	  * \param pWMesh sliced open mesh
	  */

    CBaseHeatFlow( CHCFMesh * pMesh, CHCFMesh * pWMesh );
	/*! CBaseHeatFlow destructor
	*/

    ~CBaseHeatFlow();

	/*!	Compute harmonic closed forms on the input mesh
	*/
    void calculate_harmonic_form();

  protected:
    /*! Compute the angle structure */
    virtual void _angle_structure() = 0;
	/*! Input closed mesh */	
    CHCFMesh * m_pMesh;
	/*! inpute mesh, sliced open along the shortest paths connecting two boundary loops */    
	CHCFMesh * m_pWMesh;

	CHCFMesh::CBoundary m_boundary;

	/*! Compute the harmonic 1-form */
    void _harmonic_1_form();
	/*! Integrate the closed form on the mesh */
    void _integrate();

  };
}
#endif _BASE_HEAT_FLOW_