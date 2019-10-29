/*! \file TangentialRicciExtremalLength.h
 * \brief Euclidean Tangential Ricci flow for Extremal Length
 *  \author David Gu
 *  \date   documented on 11/15/2010
 *
 *	Algorithm for Tangential Ricci Flow for Extremal Length
 */

#ifndef _TANGENTIAL_RICCI_FLOW_EXTREMAL_LENGTH_H_
#define _TANGENTIAL_RICCI_FLOW_EXTREMAL_LENGTH_H_

#include <map>
#include <vector>

#include "TangentialRicciFlow.h"

namespace MeshLib
{
	/*! \brief CInversiveDistanceRicciFlowExtremalLength class
	 *  
	 *  Algorithm for Euclidean Inversive Distance Ricci flow for Extremal Length
	 */
	 
 template<typename V, typename E, typename F, typename H>
 class CTangentialRicciFlowExtremalLength : public CTangentialRicciFlow<V,E,F,H>
  {
  public:
	  /*! \brief CTangentialRicciFlowExtremalLength constructor
	   *
	   *  call base class constructor 
	   */
	  CTangentialRicciFlowExtremalLength( CRicciFlowMesh<V,E,F,H> * pMesh );

  protected:

	/*!
	 *	Set the target curvature on each vertex
	 */
    void    _set_target_curvature();
	/*!
	 *	compute the vertex topological valence
	 */
	void _calculate_topo_valence();
  };

template<typename V, typename E, typename F, typename H>
CTangentialRicciFlowExtremalLength<V,E,F,H>::CTangentialRicciFlowExtremalLength( CRicciFlowMesh<V,E,F,H> * pMesh ): CTangentialRicciFlow( pMesh)
{
	_calculate_topo_valence();
}

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlowExtremalLength<V,E,F,H>::_calculate_topo_valence()
{
	for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
    {
      V * v = *viter;
      v->valence() = 0;
      for(CRicciFlowMesh<V,E,F,H>::VertexEdgeIterator veiter( v ); !veiter.end(); ++ veiter )
      {
        E * e = *veiter;
        if( e->sharp() ) v->valence() ++;
      }
    }
}


//set target curvature

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlowExtremalLength<V,E,F,H>::_set_target_curvature()
{
	int count = 0;
  for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
  {
    V * v = *viter;
    v->target_k() = 0;
	
	if( v->valence() > 2 )
	{
		count ++;
		v->target_k() = PI/2.0;
	}
  }
	printf("There are %d corners.\n", count );
};


}

#endif  