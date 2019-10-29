#include "HeatFlow.h"

using namespace MeshLib;


CBaseHeatFlow::CBaseHeatFlow( CHCFMesh * pMesh, CHCFMesh * pWMesh ):m_boundary( pWMesh )
{
  m_pMesh  = pMesh;
  m_pWMesh = pWMesh;
};

CBaseHeatFlow::~CBaseHeatFlow()
{
};



void CBaseHeatFlow::_harmonic_1_form()
{
    
    int vid = 0;

	for( CHCFMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
    {
        CHCFVertex * v = *viter;
        v->idx() = vid ++;
    }
   int num = m_pMesh->numVertices();

	std::vector<Eigen::Triplet<double> > A_coefficients;
	
	for( CHCFMesh::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); eiter ++ )
	{
      CHCFEdge * e = *eiter;
      CHCFVertex * v1 = m_pMesh->edgeVertex1( e );
      CHCFVertex * v2 = m_pMesh->edgeVertex2( e );

      int id1 = v1->idx();
      int id2 = v2->idx();

      double w = e->weight();
		
	  A_coefficients.push_back( Eigen::Triplet<double>(id1,id2,-w) );
	  A_coefficients.push_back( Eigen::Triplet<double>(id2,id1,-w) );

  }

	Eigen::VectorXd b( num );

	for( CHCFMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
   {
      CHCFVertex * v = *viter;
      int  id = v->idx();
		
      double sum_w = 0;
      double sum_b  = 0;

      for( CHCFMesh::VertexVertexIterator vviter( v ); !vviter.end();  ++vviter  )
      {
		  CHCFVertex * w = *vviter;
		  CHCFEdge   * e = m_pMesh->vertexEdge( v, w );

		  sum_w += e->weight();
          
          if( m_pMesh->edgeVertex1( e ) != v )
          {
			  sum_b += -( e->weight() * e->du() );
          }
          else
          {
            sum_b +=  e->weight()* e->du() ;
          }
      }
	  A_coefficients.push_back( Eigen::Triplet<double>(id,id,sum_w));

	  b(id) = sum_b;
	  if( fabs( sum_b)  > 0.25 )
      printf("(%d %f) ",  id, sum_b );
  }

  
	Eigen::SparseMatrix<double> A( num, num );
	A.setZero();
	A.setFromTriplets(A_coefficients.begin(), A_coefficients.end());


	Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
	std::cerr << "Eigen Decomposition" << std::endl;
	solver.compute(A);
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

	for( CHCFMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
	{
      CHCFVertex * v = *viter;
      int  id = v->idx();
      v->u() = x(id);
      //printf("%f ", x[id]);
	}

    for( CHCFMesh::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); eiter ++ )
    {
      CHCFEdge * e = *eiter;
      CHCFVertex * v1 = m_pMesh->edgeVertex1( e );
      CHCFVertex * v2 = m_pMesh->edgeVertex2( e );
      e->du() += v2->u() - v1->u();
   }


	CHCFMesh::CBoundary boundary( m_pMesh );
 
	for( size_t i = 0; i < boundary.loops().size(); i ++ )
	{
		CHCFMesh::CLoop * pL = boundary.loops()[i];

		std::list<CHCFHalfEdge*> pHs = pL->halfedges();
		double sum = 0; 
		  for( std::list<CHCFHalfEdge*>::iterator hiter = pHs.begin(); hiter != pHs.end(); hiter ++)
		  {
			  CHCFHalfEdge * h = *hiter;
			  CHCFEdge * e = m_pMesh->halfedgeEdge( h );
			  sum += (m_pMesh->edgeHalfedge(e,0) == h )? e->du():-e->du();
		  }
		  printf("Loop Integration %f\n", sum );
	}

}

void CBaseHeatFlow::_integrate()
{
	for( CHCFMesh::MeshEdgeIterator eiter( m_pWMesh ); !eiter.end(); eiter ++ )
  {
      CHCFEdge * we = *eiter;

      CHCFVertex * w1 = m_pWMesh->edgeVertex1( we );
      CHCFVertex * w2 = m_pWMesh->edgeVertex2( we );

	  //for diffuse exact form of multiply connected domain
	  //we use vertex id to replace vertex father

	  int id1 = ( w1->father() )? w1->father():w1->id();
	  int id2 = ( w2->father() )? w2->father():w2->id();

      CHCFVertex * v1 = m_pMesh->idVertex( id1 );
      CHCFVertex * v2 = m_pMesh->idVertex( id2 );

      CHCFEdge * e = m_pMesh->vertexEdge( v1, v2 );

      if( m_pMesh->edgeVertex1( e ) == v1 )
      {
          we->du() = e->du();
      }
      else
      {  
        we->du() = - e->du();
      }
  }

  CHCFVertex * head = NULL;
  for( CHCFMesh::MeshVertexIterator viter( m_pWMesh ); !viter.end(); viter ++ )
  {
      CHCFVertex * v = *viter;
      v->touched() = false;
      head = v;
  }

  std::queue<CHCFVertex*> vqueue;
  head->touched() = true;
  vqueue.push( head );
  head->uv() = CPoint2(0,0.27);

  while( !vqueue.empty() )
  {
    CHCFVertex * head = vqueue.front();
    vqueue.pop();

	for( CHCFMesh::VertexEdgeIterator veiter(  head ); !veiter.end(); veiter ++ )
    {
      CHCFEdge * e = *veiter;
      double du = e->du();

      CHCFVertex * v1 = m_pWMesh->edgeVertex1( e );
      CHCFVertex * v2 = m_pWMesh->edgeVertex2( e );

      if( v1 == head )
      {
        if( v2->touched() ) continue;
        v2->touched() = true;
        vqueue.push( v2 );
        v2->uv() = v1->uv() + CPoint2( du,0 );
      }
      else
      {
        assert( v2 == head );
        if( v1->touched() ) continue;
        v1->touched() = true;
        vqueue.push( v1 );
        v1->uv() = v2->uv() - CPoint2( du,0 );
      }
    }
  }
}

void CBaseHeatFlow::calculate_harmonic_form()
{
	_angle_structure();
	_harmonic_1_form();
    _integrate();
}
