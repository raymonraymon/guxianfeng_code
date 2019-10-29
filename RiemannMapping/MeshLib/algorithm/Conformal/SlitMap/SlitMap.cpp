/*! \file SlitMap.cpp
*  \brief Implementation  of CSlitMap class. Algorithm for computing slit mapping
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

#include "SlitMap.h"

using namespace MeshLib;

//CSlitMap constructor
//\param meshes the input holomorphic 1-form basis

CSlitMap::CSlitMap( std::vector<CSMMesh*> & meshes ): m_meshes( meshes ),m_pMesh( meshes[0]), m_boundary( m_pMesh )
{
	assert( m_meshes.size() > 0 );
}

//CSlitMap destructor
CSlitMap::~CSlitMap()
{
}

//Integrate a holomorphic 1-form pMesh along a boundary loop pL

double CSlitMap::_integrate( CSMMesh * pMesh, CSMMesh::CLoop * pL )
{
	CPoint2 sum(0,0);
	
	for( std::list<CHalfEdge*>::iterator hiter = pL->halfedges().begin();hiter != pL->halfedges().end(); hiter ++ )
	{
		CHalfEdge * he = *hiter;
		CVertex * v1 = he->source();
		CVertex * v2 = he->target();

		int id1 = v1->id();
		int id2 = v2->id();

		CVertex * w1 = pMesh->idVertex( id1 );
		CVertex * w2 = pMesh->idVertex( id2 );

		CSlitMapEdge * e = pMesh->vertexEdge( w1, w2 );

		if( pMesh->edgeVertex1( e ) == w1 )
		{
			sum = sum + e->duv();
		}
		else
		{
			sum = sum - e->duv();
		}
	}

	return sum[1];
}

//Compute the slit map, by finding a holomorphic 1-form, the integration along c1 loop is 2PI, the integration
//along c2 is -2PI

void CSlitMap::_slit_map(int c1,int c2 ) 
{

	std::vector<CSMMesh::CLoop*> & loops = m_boundary.loops();

	int num = m_meshes.size();

	if( c1 < 0 || c2 < 0 || (size_t) c1 > loops.size()  || (size_t) c2 > loops.size() )
	{
		printf( "The loop Id should between [0..%d]\n", loops.size() );
		return;
	}

	Eigen::MatrixXd A = Eigen::MatrixXd::Zero( num, num );
	Eigen::VectorXd b( num );
	b.setZero();


	for( int i = 0; i < num; i ++ )
	{
		//CSMMesh::CLoop * pL = loops[i+1];
		CSMMesh::CLoop * pL = loops[i];
		
		for( int j = 0; j < num; j ++ )
		{
			CSMMesh * pM = m_meshes[j];
			double w = _integrate( pM, pL );
			A(i,j) = w;
		}
		b(i) = 0;
	}

	b(c1) = 1;
	if( num > 1 )
	{
		b(c2) = -1;
	}

	//Eigen::VectorXd x = solver.solve(b);
	Eigen::VectorXd x = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

	for( CSMMesh::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); ++ eiter )
	{
		CSlitMapEdge * e = *eiter;
		CVertex * v1 = m_pMesh->edgeVertex1( e );
		CVertex * v2 = m_pMesh->edgeVertex2( e );
		CPoint2 duv(0,0);

		for( size_t k = 0; k < m_meshes.size(); k ++ )
		{
			CSMMesh * pM = m_meshes[k];
			CVertex * w1 = pM->idVertex( v1->id() );
			CVertex * w2 = pM->idVertex( v2->id() );
			CSlitMapEdge * we = pM->vertexEdge( w1, w2 );
			CPoint2 d = we->duv();
			d = d * x(k);
			duv = duv + d;
		}
		e->duv() = duv;
	}
}

