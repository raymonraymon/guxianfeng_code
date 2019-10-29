/*! \file Integration.cpp
*   \brief Implementation of CIntegration class
*	\author David Gu
*   \date Documented on 10/12/2010
*
*	Compute the integration of a holomorphic 1-form on a domain mesh, the results are stored in vertex uv field.
*/

#include "Integration.h"

using namespace MeshLib;

//CIntegration constructor
//\param pForm the input holomorphic 1-form
//\param pDomain the domain mesh with vertex uv

CIntegration::CIntegration( CIMesh * pForm, CIMesh * pDomain )
{
	m_pForm = pForm;
	m_pDomain = pDomain;

};

//CIntegration destructor
CIntegration::~CIntegration()
{
};


//Compute the integration
//Using breadth first search 

void CIntegration::_integrate()
{

	CIntegrationVertex * head = NULL;

	for( CIMesh::MeshVertexIterator viter( m_pDomain ); !viter.end(); viter ++ )
	{
		CIntegrationVertex * v = *viter;
		v->touched() = false;
		head = v;
	}


	std::queue<CIntegrationVertex*> vqueue;
	head->uv() = CPoint2(0,0);
	head->touched()  = true;

	vqueue.push( head );

	while( !vqueue.empty() )
	{
		head = vqueue.front();
		vqueue.pop();

		for( CIMesh::VertexEdgeIterator veiter( head ); !veiter.end(); veiter ++ )
		{
			CIntegrationEdge * e = *veiter;
			CIntegrationVertex * v1 = m_pDomain->edgeVertex1( e );
			CIntegrationVertex * v2 = m_pDomain->edgeVertex2( e );


			CIntegrationVertex * tail = ( head != v1 )?v1:v2;
			if( tail->touched() ) continue;

			tail->touched() = true;
			vqueue.push( tail );

			int id1 = v1->father();
			//if there is no "father" field for the vertex, then directly use the vertex id 
			if( id1 == 0 ) id1 = v1->id();
			int id2 = v2->father();
			//if there is no "father" field for the vertex, then directly use the vertex id 
			if( id2 == 0 ) id2 = v2->id();

			CIntegrationVertex * w1 = m_pForm->idVertex( id1 );
			CIntegrationVertex * w2 = m_pForm->idVertex( id2 );

			CIntegrationEdge * we = m_pForm->vertexEdge( w1, w2 );

			if( m_pForm->edgeVertex1( we ) == w1 )
			{
				e->duv() = we->duv();
			}
			else
			{
				e->duv() = CPoint2(0,0) - we->duv();
			}

			if( tail == v2 )
			{
				tail->uv()  = head->uv() + e->duv();
			}
			else
			{
				tail->uv()  = head->uv() - e->duv();
			}
		}
	}
	
}

