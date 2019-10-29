/*!
*      \file BaseHarmonicExactForm.cpp
*      \brief Implement CHarmonicExactForm class
*	   \author David Gu
*      \date Document 10/11/2010
*
*		Compute exact harmonic forms on genus zero surfaces with multiple boundary components
*
*/

#include "BaseHarmonicExactForm.h"

using namespace MeshLib;


//CBaseHarmonicExactForm constructor
// pMesh is the input mesh
CBaseHarmonicExactForm::CBaseHarmonicExactForm( CHarmonicMesh * pMesh ):m_pMesh( pMesh ), m_boundary( m_pMesh )
{

	int vid  = 0;
	int vfid = 0; //fixed vertex id

	for( CHarmonicMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
	{
		CHVertex * pV = *viter;
		if( pV->boundary() )
		{
			pV->idx() = vfid ++;
		}
		else
		{
			pV->idx() = vid  ++;
		}
	}

	m_interior_vertices = vid;
	m_boundary_vertices = vfid;

}


//CHarmonicExactForm destructor

CBaseHarmonicExactForm::~CBaseHarmonicExactForm()
{
}


//Compute the harmonic exact form, such that the harmonic function
//equals to one on the boundary loop pL

void CBaseHarmonicExactForm::_harmonic_exact_form( CHarmonicMesh::CHLoop * pL )
{
	for( CHarmonicMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
	{
		CHVertex * pV = *viter;
		pV->u() = 0.0;
	}

	for( std::list<CHHalfEdge*>::iterator hiter = pL->halfedges().begin(); hiter != pL->halfedges().end(); hiter ++ )
	{
		CHHalfEdge * he = *hiter;
		CHVertex   * pV = m_pMesh->halfedgeVertex( he );
		pV->u() = -1.0;
		//pV->u() = 1.0;
	}

	std::vector<Eigen::Triplet<double> > A_coefficients;
	std::vector<Eigen::Triplet<double> > B_coefficients;

	for( CHarmonicMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
	{
		CHVertex * pV = *viter;
		if( pV->boundary() ) continue;
		int vid = pV->idx();

		double sw = 0;
		//double sb = 0;

		for( CHarmonicMesh::VertexVertexIterator witer( pV ); !witer.end(); ++ witer )
		{
			CHVertex * pW = *witer;
			int wid = pW->idx();
			
			CHEdge * e = m_pMesh->vertexEdge( pV, pW );
			double w = e->weight();

			if( pW->boundary() )
			{
				B_coefficients.push_back( Eigen::Triplet<double>( vid, wid, w ) );
			}
			else
			{
				A_coefficients.push_back( Eigen::Triplet<double>( vid, wid, -w ) );
			}
			sw += w;
		}
		A_coefficients.push_back( Eigen::Triplet<double>( vid, vid, sw ) );
	}

	Eigen::SparseMatrix<double> A( m_interior_vertices, m_interior_vertices );
	A.setZero();

	Eigen::SparseMatrix<double> B( m_interior_vertices, m_boundary_vertices );
	B.setZero();
	A.setFromTriplets(A_coefficients.begin(), A_coefficients.end());
	B.setFromTriplets(B_coefficients.begin(), B_coefficients.end());


	Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
	std::cerr << "Eigen Decomposition" << std::endl;
	solver.compute(A);
	std::cerr << "Eigen Decomposition Finished" << std::endl;
	
	if( solver.info() != Eigen::Success )
	{
		std::cerr << "Waring: Eigen decomposition failed" << std::endl;
	}

	Eigen::VectorXd b(m_boundary_vertices);
	for( CHarmonicMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
	{
		CHVertex * pV = *viter;
		if( !pV->boundary() ) continue;
		int id = pV->idx();
		b(id)  = pV->u();
		//printf("%f\n", b[id]);
	}

	Eigen::VectorXd c = B * b;
	Eigen::VectorXd x = solver.solve(c);
	if( solver.info() != Eigen::Success )
	{
		std::cerr << "Waring: Eigen decomposition failed" << std::endl;
	}
	
	for( CHarmonicMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
	{
		CHVertex * pV = *viter;
		if( pV->boundary() )
		{
			pV->uv() = CPoint2( pV->u(), 0.53 );
			continue;
		}
		int id = pV->idx();
		pV->u()  = x(id);
		pV->uv() = CPoint2( pV->u(), 0.53 );
	}

	for( CHarmonicMesh::MeshEdgeIterator eiter( m_pMesh ); !eiter.end(); eiter ++ )
	{
      CHEdge * e = *eiter;
      CHVertex * v1 = m_pMesh->edgeVertex1( e );
      CHVertex * v2 = m_pMesh->edgeVertex2( e );
      e->du() = v2->u() - v1->u();
	}
}

//Compute harmonic exact forms 
//output the results to the meshes "prefix_k.du.m"

void CBaseHarmonicExactForm::calculate_harmonic_exact_form(const char * prefix )
{

	_angle_structure();		

	std::vector<CHarmonicMesh::CHLoop*>& loops = m_boundary.loops();

	for( size_t k = 1; k < loops.size(); k ++ )
	{
		printf("%f \n", loops[k]->length());
	}

	for( size_t k = 1; k < loops.size(); k ++ )
	{		
	  _harmonic_exact_form( loops[k] );

	  std::stringstream iss;
	  iss << prefix << "_" << k-1 << ".du.m ";
	  m_pMesh->write_m( iss.str().c_str() );
	}
}


