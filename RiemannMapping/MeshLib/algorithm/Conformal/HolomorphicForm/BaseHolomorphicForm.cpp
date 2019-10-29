/*! \file BaseHolomorphicForm.cpp
*   \brief Implementation of BaseHolomorphicForm class
*
*   Computing Hodge Star operator and holomorphic 1-form basis on the mesh.
*   
*/
#include "BaseHolomorphicForm.h"

using namespace MeshLib;


//-----------------------------------------------------------------------------
//Wedge product

double CWedgeOperator::wedge_product()
{

	double p = 0;
	for( CHoloFormMesh::MeshFaceIterator fiter( m_pMesh[0] ); !fiter.end(); ++ fiter )
	{
		CHoloFormFace * f0 = *fiter;
		CHoloFormFace * f1 = m_pMesh[1]->idFace( f0->id() );
		
		std::vector<CHoloFormHalfEdge*> h0,h1;
		for( CHoloFormMesh::FaceHalfedgeIterator fhiter( f0 ); !fhiter.end(); ++fhiter )
		{
			CHoloFormHalfEdge * h = *fhiter;
			h0.push_back( h );
		}

		for( CHoloFormMesh::FaceHalfedgeIterator fhiter( f1 ); !fhiter.end(); ++fhiter )
		{
			CHoloFormHalfEdge * h = *fhiter;
			h1.push_back( h );
		}

		CVertex *  s0 = m_pMesh[0]->halfedgeSource( h0.front() );
		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h1.front();
			CVertex *   s = m_pMesh[1]->halfedgeSource( h );
			if( s->id() != s0->id() )
			{
				h1.erase( h1.begin() );
				h1.push_back( h );
			}
			else
			{
				break;
			}
		}

		std::vector<double> du0,du1;
		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h0[i];
			CHoloFormEdge * e = m_pMesh[0]->halfedgeEdge( h );
			double du = ( h == m_pMesh[0]->edgeHalfedge( e, 0 )  )? e->du():-e->du();
			du0.push_back( du );	
		}

		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h1[i];
			CHoloFormEdge * e = m_pMesh[1]->halfedgeEdge( h );
			double du = ( h == m_pMesh[1]->edgeHalfedge( e, 0 )  )? e->du():-e->du();
			du1.push_back( du );	
		}


		
		//	 |du0[0] du0[1] du0[2]|
		//	 |du1[0] du1[1] du1[2]| /6
		//	 |1      1      1     |
		 
		p += ( du0[1] * du1[2] - du0[2] * du1[1] ) /2.0;
	}
	return p;
};

//Hodge Star operator

double CWedgeOperator::wedge_star_product()
{

	double p = 0;
	for( CHoloFormMesh::MeshFaceIterator fiter( m_pMesh[0] ); !fiter.end(); ++ fiter )
	{
		CHoloFormFace * f0 = *fiter;
		CHoloFormFace * f1 = m_pMesh[1]->idFace( f0->id() );

		std::vector<CHoloFormHalfEdge*> h0,h1;
		for( CHoloFormMesh::FaceHalfedgeIterator fhiter( f0 ); !fhiter.end(); ++fhiter )
		{
			CHoloFormHalfEdge * h = *fhiter;
			h0.push_back( h );
		}

		for( CHoloFormMesh::FaceHalfedgeIterator fhiter( f1 ); !fhiter.end(); ++fhiter )
		{
			CHoloFormHalfEdge * h = *fhiter;
			h1.push_back( h );
		}

		CVertex *  s0 = m_pMesh[0]->halfedgeSource( h0.front() );
		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h1.front();
			CVertex *   s = m_pMesh[1]->halfedgeSource( h );
			if( s->id() != s0->id() )
			{
				h1.erase( h1.begin() );
				h1.push_back( h );
			}
			else
			{
				break;
			}
		}

		std::vector<double> du0,du1,theta;
		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h0[i];
			CHoloFormEdge * e = m_pMesh[0]->halfedgeEdge( h );
			double du = ( h == m_pMesh[0]->edgeHalfedge( e, 0 )  )? e->du():-e->du();
			du0.push_back( du );	
			theta.push_back( h0[(i+1)%3]->angle() );
		}

		for( size_t i = 0; i < 3; i ++ )
		{
			CHoloFormHalfEdge * h = h1[i];
			CHoloFormEdge * e = m_pMesh[1]->halfedgeEdge( h );
			double du = ( h == m_pMesh[1]->edgeHalfedge( e, 0 )  )? e->du():-e->du();
			du1.push_back( du );	
		}
	

		p += 0.5 * du0[0] * du1[0] * cos( theta[0] )/sin( theta[0] );
		p += 0.5 * du0[1] * du1[1] * cos( theta[1] )/sin( theta[1] );
		p += 0.5 * du0[2] * du1[2] * cos( theta[2] )/sin( theta[2] );
	}
	return p;
};

//CWedgeOperator constructor
//\param mesh0, mesh1 two input harmonic 1-forms

CWedgeOperator::CWedgeOperator(CHoloFormMesh * mesh0, CHoloFormMesh * mesh1)
{
	m_pMesh[0] = mesh0;
	m_pMesh[1] = mesh1;
};

//CWedgeOperator destructor

CWedgeOperator::~CWedgeOperator()
{

};

//CBaseHolomorphicForm constructor
//\param meshes are the basis of harmonic 1-form group

CBaseHolomorphicForm::CBaseHolomorphicForm( std::list<CHoloFormMesh*> & meshes )
{
	for( std::list<CHoloFormMesh*>::iterator miter = meshes.begin(); miter != meshes.end(); miter ++ )
	{
		CHoloFormMesh * pM = *miter;
		m_meshes.push_back( pM );
	}
};

//CBaseHolomorphicForm destructor

CBaseHolomorphicForm::~CBaseHolomorphicForm()
{
};


// Compute the conjugate harmonic 1-forms for the base harmonic 1-forms

void CBaseHolomorphicForm::conjugate()
{

	_angle_structure();

	int n = m_meshes.size();


	Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);

	for(int i = 0;  i < n; i ++ )
	{
		for( int j = 0;  j < n; j ++ )
		{
				CWedgeOperator wo( m_meshes[i], m_meshes[j] );
				A(i,j) = wo.wedge_product();
		}
	}



	for(int i = 0; i < n ; i ++ )
	{
		Eigen::VectorXd b(n);
		
		for( int j = 0; j < n ; j ++ )
		{
			CWedgeOperator wo( m_meshes[i], m_meshes[j] );
			b(j) = wo.wedge_star_product();
		}

		
		//Eigen::VectorXd x = solver.solve(b);
		Eigen::VectorXd x = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

		
		std::cout << "Hodge Star Coefficients: ";
		for( int j = 0; j < n ; j ++ )
		{
			std::cout << x(j) << " ";
		}
		std::cout << std::endl;

		for( CHoloFormMesh::MeshEdgeIterator eiter( m_meshes[i] ); !eiter.end(); ++ eiter )
		{
			CHoloFormEdge * e = *eiter;
			e->duv()[0] = e->du();
			e->duv()[1] = 0;
		
			int id1 = m_meshes[i]->edgeVertex1(e)->id();
			int id2 = m_meshes[i]->edgeVertex2(e)->id();

			for( int k = 0; k < n ; k ++ )
			{
				CHoloFormVertex * w1 = m_meshes[k]->idVertex( id1 );
				CHoloFormVertex * w2 = m_meshes[k]->idVertex( id2 );

				CHoloFormEdge * edge = m_meshes[k]->vertexEdge( w1, w2 );
				e->duv()[1] += edge->du() * x(k);
			}

		}

	}

};


