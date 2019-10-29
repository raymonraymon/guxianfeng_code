/*! \file BaseRicciFlow.h
 *  \brief Base class for Ricci flow algorithm
 *  \author David Gu
 *  \date   documented on 10/17/2010
 *
 *	Algorithm for general Ricci Flow
 */

#ifndef _BASE_RICCI_FLOW_H_
#define _BASE_RICCI_FLOW_H_

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

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace MeshLib
{
/*! \brief BaseClass CBaseRicciFlow
*
*	Algorithm for computing general Ricci flow
*/
template<typename V, typename E, typename F, typename H>
  class CBaseRicciFlow
  {
  public:
    /*! \brief CBaseRicciFlow constructor
	 *  \param pMesh the input mesh
	 */
    CBaseRicciFlow( CRicciFlowMesh<V,E,F,H> * pMesh );
    /*! \brief CBaseRicciFlow destructor
	 */
    ~CBaseRicciFlow();
	/*!	Computing the metric
	 */
	virtual void _calculate_metric();


  protected:
    /*!
     *	the input mesh
	 */
    CRicciFlowMesh<V,E,F,H>	  * m_pMesh;
	/*!
	 *	boundary of the input mesh
	 */
	CBoundary<V,E,F,H>		  m_boundary;

  protected:
	  /*!
	   *	Calculate each edge length, has to be defined in the derivated classes
	   */
   virtual void _length( double u1, double u2, E * e )=0;
	  /*!
	   *	Calculate each edge length
	   */
    void _calculate_edge_length();

	/*!
	 *	Cosine law, has to be defined in the derivated classes
	 */
	virtual double _cosine_law( double a, double b, double c ) = 0;
	/*!
	 *	Calculate corner angle
	 */
    void _calculate_corner_angle();
	/*!
	 *	Calculate vertex curvature
	 */
    void _calculate_vertex_curvature();
	/*!
	 *	Calculate vertex curvature error
	 */
   double   _calculate_curvature_error();

	/*!
	 *	Calculate the edge weight
	 */
    virtual void _calculate_edge_weight() = 0;

	/*!
	 *	Set the target curvature on each vertex
	 */
    virtual void  _set_target_curvature() =  0;
    /*!
	 *	Curvature flow 
	 */
     virtual bool   _flow( double );
	 /*!
	  *	Newton's method to optimize the entropy energy
	  * \param threshold err bound
	  * \param step_length step length
	  */
     virtual void   _Newton( double threshold, double step_length );
	 /*!
	  *	Normalization
	  * \param du the du vector
	  * \param n dimension of the du vector
	  */
	 virtual void _normalization( Eigen::VectorXd & du, int n ) = 0;
	 /*!
	  *	calculate hessian matrix Hessain 
	  * \param SparseMatrix
	  */
	 virtual void _calculate_Hessain( Eigen::SparseMatrix<double> & pMatrix );
  };

//Constructor
template<typename V, typename E, typename F, typename H>
CBaseRicciFlow<V,E,F,H>::CBaseRicciFlow( CRicciFlowMesh<V,E,F,H> * pMesh ): m_pMesh( pMesh), m_boundary( pMesh )
{
  int idx = 0;
  for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
	{
      V * v = *viter;
      v->u() = 0;
      v->idx() = idx ++; 
	}
};

//Destructor
template<typename V, typename E, typename F, typename H>
CBaseRicciFlow<V,E,F,H>::~CBaseRicciFlow( )
{
};

//Compute the edge length
template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_calculate_edge_length( )
{

	for( CRicciFlowMesh<V,E,F,H>::MeshEdgeIterator eiter(m_pMesh);  !eiter.end();  eiter++ )
  {
      E * e = *eiter;

      V * v1 = m_pMesh->edgeVertex1( e );
      V * v2 = m_pMesh->edgeVertex2( e );

      double u1 = v1->u();
      double u2 = v2->u();
	  
	  _length( u1, u2, e );
  }

};


//Calculate corner angle
template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_calculate_corner_angle( )
{
	//CStructure<CRicciFlowMesh<V,E,F,H>, V,E,F,H> pS( m_pMesh );
	//pS._metric_2_angle();
	
	for ( CRicciFlowMesh<V,E,F,H>::MeshFaceIterator fiter( m_pMesh); ! fiter.end(); fiter ++ )
  {
      F * f = *fiter;

      H * he[3];

      he[0] = m_pMesh->faceMostCcwHalfEdge( f ); 

	  for( int i = 0; i < 3; i ++ )
      {
        he[(i+1)%3] = m_pMesh->	faceNextCcwHalfEdge(he[i]);
      }

      double l[3];
      for(int i = 0; i < 3; i ++ )
      {
		  E * e = m_pMesh->halfedgeEdge( he[i] );
          l[i] = e->length();
      }

      for(int i = 0; i < 3; i ++ )
      {
          he[(i+1)%3]->angle() = _cosine_law( l[(i+1)%3] ,l[(i+2)%3] ,l[i] );
      }
  }

};

//Calculate vertex curvature
template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_calculate_vertex_curvature()
{

  for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
  {
      V * v = *viter;
      double k  = (v->boundary() )? PI: PI * 2;
	  for( CRicciFlowMesh<V,E,F,H>::VertexInHalfedgeIterator vh( m_pMesh, v ); !vh.end();  ++vh )
      {
          H * he = *vh;
          k -= he->angle();
      }
      v->k() = k;
  }

};


//compute curvature error

template<typename V, typename E, typename F, typename H>
double CBaseRicciFlow<V,E,F,H>::_calculate_curvature_error()
{
  double max_error = -1;
  V * vert = NULL;

  for(CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh); !viter.end() ; viter ++ )
   {
       V * v = *viter;
	   double k = v->target_k() - v->k();      
       k = fabs( k );
	   if( k > max_error )
	   {
		max_error = k;
		vert = v;
	   }
   }
   printf("Vertex id is %d\n", vert->id() );
   return max_error; 
};

//compute metric

template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_calculate_metric()
{

   _set_target_curvature();
  //double error = 1e-6;
  double threshold = 5e-4;
  double step_length = 1.0;
   _Newton( threshold, step_length );     

};

//gradient flow method

template<typename V, typename E, typename F, typename H>
bool CBaseRicciFlow<V,E,F,H>::_flow( double error_threshold )
{
  int num = m_pMesh->numVertices();

  for( int k = 0; k < 64000; k ++  )
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

//Newton's method for optimizing entropy energy

template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_Newton( double threshold, double step_length )
{
	int num = m_pMesh->numVertices();

	
  	while( true )
	{
		//the order of the following functions really matters

 		_calculate_edge_length();
		_calculate_corner_angle();
		_calculate_vertex_curvature();
		_calculate_edge_weight();

		double error =  _calculate_curvature_error();
		printf("Newton's Method: Current error is %f\r\n", error );

		if( error < threshold) break;
	

		Eigen::SparseMatrix<double>  M( num, num );
		M.setZero();
		_calculate_Hessain( M );

		Eigen::VectorXd b(num);

		for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++  )
		{
		    V * v = *viter;
			int idx = v->idx();
			b(idx) = v->target_k() - v->k();
		}

		//std::cout << M;
		//Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
		std::cerr << "Eigen Decomposition" << std::endl;
		solver.compute(M);
		std::cerr << "Eigen Decomposition Finished" << std::endl;
	
		if( solver.info() != Eigen::Success )
		{
			std::cerr << "Waring: Eigen decomposition failed" << std::endl;
		}

		Eigen::VectorXd x = solver.solve(b);
		if( solver.info() != Eigen::Success )
		{
			std::cerr << "Waring: Eigen decomposition failed" << std::endl;
		}
		//std::cout << x.transpose();

		_normalization( x, num );

		for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++  )
		{
		    V * v = *viter;
			int idx = v->idx();
			v->u() += x(idx) * step_length;
		}

  }

};


//Newton's method for optimizing entropy energy

template<typename V, typename E, typename F, typename H>
void CBaseRicciFlow<V,E,F,H>::_calculate_Hessain( Eigen::SparseMatrix<double> & M )
{
	std::vector<Eigen::Triplet<double> > M_coefficients;

	//set A
	for( CRicciFlowMesh<V,E,F,H>::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); eiter ++  )
	{
	  E * e = *eiter;
	  V * v1 = m_pMesh->edgeVertex1( e );
	  V * v2 = m_pMesh->edgeVertex2( e );

	  M_coefficients.push_back( Eigen::Triplet<double>( v1->idx(), v2->idx(), -e->weight()) );
	  M_coefficients.push_back( Eigen::Triplet<double>( v2->idx(), v1->idx(), -e->weight()) );

	}
  
	for( CRicciFlowMesh<V,E,F,H>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++  )
	{
	    V * v = *viter;
		int idx = v->idx();
		double w = 0;
		for( CRicciFlowMesh<V,E,F,H>::VertexEdgeIterator veiter( v ); !veiter.end(); veiter ++ )
		{
		  E * pE = *veiter;
		  w += pE->weight();
		}
		M_coefficients.push_back( Eigen::Triplet<double>( idx, idx, w ) );
  }

  M.setFromTriplets(M_coefficients.begin(), M_coefficients.end());
}

}
#endif  _BASE_RICCI_FLOW_H_