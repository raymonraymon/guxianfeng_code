/*! \file TangentialRicciFlow.h
 *  \brief General Euclidean Ricci flow algorithm
 *  \author David Gu
 *  \date   documented on 10/17/2010
 *
 *	Algorithm for general Ricci Flow
 */

#ifndef _TANGENTIAL_RICCI_FLOW_H_
#define _TANGENTIAL_RICCI_FLOW_H_

#include <map>
#include <vector>
#include <Eigen/Sparse>

#include "Mesh/BaseMesh.h"
#include "Mesh/Vertex.h"
#include "Mesh/HalfEdge.h"
#include "Mesh/Edge.h"
#include "Mesh/Face.h"
#include "mesh/iterators.h"
#include "mesh/boundary.h"
#include "Parser/parser.h"
#include "RicciFlowMesh.h"
#include "BaseRicciFlow.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace MeshLib
{
/*! \brief Class CTangentialRicciFlow
*
*	Algorithm for computing Ricci flow
*/
template<typename V, typename E, typename F, typename H>
class CTangentialRicciFlow : public CBaseRicciFlow<V,E,F,H>
  {
  public:
    /*! \brief CTangentialRicciFlow constructor
	 *  \param pMesh the input mesh
	 */
	  CTangentialRicciFlow( CRicciFlowMesh<V,E,F,H> * pMesh );
    /*! \brief CTangentialRicciFlow destructor
	 */
	  ~CTangentialRicciFlow(){};
	/*!	Computing the metric
	 */
	void _calculate_metric();
	 /*!
	 *	Curvature flow, override 
	 */
     bool _flow( double );

  protected:

	  /*!
	   *	Calculate each edge length, has to be defined in the derivated classes
	   */
   void _length( double u1, double u2, E * e );

	/*!
	 *	Cosine law, has to be defined in the derivated classes
	 */
	double _cosine_law( double a, double b, double c ) ;
	 /*!
	  *	Normalization
	  * \param du the du vector
	  * \param n dimension of the du vector
	  */
	void _normalization( Eigen::VectorXd & du, int n );


	/*!
	 *	Calculate the edge weight
	 */
    void _calculate_edge_weight();

	/*!
	 *	Set the target curvature on each vertex
	 */
    virtual void    _set_target_curvature();

  };


template<typename V, typename E, typename F, typename H>
CTangentialRicciFlow<V,E,F,H>::CTangentialRicciFlow( CRicciFlowMesh<V,E,F,H> * pMesh ):CBaseRicciFlow( pMesh)
{
};

//Compute the edge length
template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlow<V,E,F,H>::_length( double u1, double u2, E * e )
{
	  e->length() = exp(u1) + exp(u2);
};


//Calculate corner angle
template<typename V, typename E, typename F, typename H>
double CTangentialRicciFlow<V,E,F,H>::_cosine_law( double a, double b, double c )
{
          double cs =  ( a*a + b * b  - c * c )/( 2.0 * a * b );
          assert( cs <= 1.0 && cs >= -1.0 );
          return acos( cs );    
};


//Calculate edge weight

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlow<V,E,F,H>::_calculate_edge_weight()
{
	for( CRicciFlowMesh<V,E,F,H>::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); eiter ++ )
  {
      E * e = *eiter;
      e->weight() = 0.0;
  }

	for(  CRicciFlowMesh<V,E,F,H>::MeshFaceIterator fiter( m_pMesh ) ; !fiter.end(); fiter ++ )
  {
      F * f = *fiter;

      double r[3];
      int i = 0;
	  for( CRicciFlowMesh<V,E,F,H>::FaceHalfedgeIterator hiter(  f ); !hiter.end(); ++hiter )
      {
         H * he = *hiter;
		 V * v = m_pMesh->halfedgeTarget(he);
         r[i++] = exp( v->u() );
      }

      double w = sqrt(r[0]*r[1]*r[2]/(r[0]+r[1]+r[2]));
      
	  for( CRicciFlowMesh<V,E,F,H>::FaceEdgeIterator eiter(f); !eiter.end(); ++ eiter )
      {
          E * e = * eiter;
          e->weight() += w/e->length();
      }
  }
};

//set target curvature

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlow<V,E,F,H>::_set_target_curvature()
{
  for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
  {
    V * v = *viter;
    v->target_k() = 0;
  }

  std::vector<CRicciFlowMesh<V,E,F,H>::CLoop*>& pLs = m_boundary.loops();
	
  int id = 0;

  for( std::vector<CRicciFlowMesh<V,E,F,H>::CLoop*>::iterator liter = pLs.begin(); liter != pLs.end(); liter++)
  {
	  CRicciFlowMesh<V,E,F,H>::CLoop * pL = *liter;
	  std::list<H*> & pHes = pL->halfedges();

	  double sum = 0;
	  double inv_sum = 0;
	
	  for( std::list<H*>::iterator hiter = pHes.begin(); hiter != pHes.end(); hiter ++ )
	  {
			H  * he = *hiter;
			E  * pe = m_pMesh->halfedgeEdge( he );

			sum  += pe->length();
			inv_sum += 1/pe->length();
 	  }

	  for( std::list<H*>::iterator hiter = pHes.begin(); hiter != pHes.end(); hiter ++ )
	  {
			H * ce = *hiter;
			V   * pv = m_pMesh->halfedgeTarget( ce );
			H * he = m_pMesh->vertexMostCcwInHalfEdge( pv );
			H * te =  m_pMesh->vertexMostClwOutHalfEdge( pv );

			double L = ( m_pMesh->halfedgeEdge(he)->length() + m_pMesh->halfedgeEdge( te )->length()  )/2.0;

		 // map all boundaries to circular holes
			if( id == 0 )
			{
				double tk = 2*PI*L/sum;
				pv->target_k() = tk;
			}
			else
			{
				double tk = -2*PI*L/sum;
				pv->target_k() = tk;
			}
	  }
	  id ++;
  }

};

//compute metric

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlow<V,E,F,H>::_calculate_metric()
{

  //double error = 1e-6;
  double error = 5e-4;

  _calculate_edge_length();

  while( true )
  {
	  _set_target_curvature();
	  _Newton( error, 1 );
    //break;
      if( _flow( error ) ) break;
}      


};

//gradient flow method

template<typename V, typename E, typename F, typename H>
bool CTangentialRicciFlow<V,E,F,H>::_flow( double error_threshold )
{
  int num = m_pMesh->numVertices();

  for( int k = 0; k < 64; k ++  )
	  {
 		  _calculate_edge_length();
	      _set_target_curvature();
		  _calculate_edge_weight();

		  _calculate_corner_angle();
		  _calculate_vertex_curvature();

		  double error =  _calculate_curvature_error();
		  printf("Current error is %f\r\n", error );

		  if( error < error_threshold)  return true;
  	  

	  //set b 
		  for(CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
		  {
		    V * v = *viter;
			double dif = v->target_k() - v->k();
            v->u() += dif * 2e-2;
		  }
    }
    return false;
};

//normalization

template<typename V, typename E, typename F, typename H>
void CTangentialRicciFlow<V,E,F,H>::_normalization( Eigen::VectorXd & x, int num )
{

	double s = 0;
	for(int i = 0; i < num; i ++ )
	{
		s += x(i);
	}
	s /= num;

	for (int i = 0; i < num; i++)
	{
	 x(i) -= s;
	}
};

}

#endif  _TANGENTIAL_RICCI_FLOW_H_