/*!
*      \file Puncture.cpp
*      \brief Implement CPuncture class
*	   \author David Gu
*      \date Documented 10/12/2010
*
*		Puncture a hole in the center of the mesh, and the reverse operation to fill the hole.
*	    This algorithm is used for computing Riemann mapping via slit mapping method.
*/

#include "Puncture.h"

using namespace MeshLib;

//CPuncture constructor
//\param pMesh the input mesh
CPuncture::CPuncture( CPMesh * pMesh ): m_pMesh( pMesh ), m_boundary( m_pMesh )
{
}

//CPuncture destructor
CPuncture::~CPuncture()
{
}

//puncture a hole in the center of the mesh
//we use bread first search method, to find the most inner vertex on the mesh
// and treat it as the center of the mesh.

void CPuncture::_puncture()
{
/*
	if( m_boundary.loops().size() != 1 )
	{
		fprintf(stderr, "Input mesh should be a topological disk\n");
		return;
	}
*/
    std::queue<CPunctureVertex*> vqueue;

	for( CPMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); ++ viter )
    {
      CPunctureVertex * v = *viter;
	  if( !v->boundary() )
		 v->touched() = false;
	  else
	  {
		  v->touched() = true;
		  vqueue.push( v );
	  }
	
    }
    
    CPunctureVertex * destiny = NULL;


    while( ! vqueue.empty() )
    {
        CPunctureVertex * head = vqueue.front();
        vqueue.pop();
		destiny = head;		        
		for( CPMesh::VertexVertexIterator vviter( head ); !vviter.end(); ++ vviter )
        {
            CPunctureVertex * w = *vviter;
            if( w->touched() ) continue;
            w->touched() = true;
            vqueue.push( w );
        }
    }

	printf("Find the center vertex %d\n", destiny->id() );

	CFace * punched_face = NULL;

	for( CPMesh::VertexFaceIterator vfiter( destiny ); !vfiter.end(); vfiter ++ )
	{
		CFace * pF = *vfiter;
		punched_face = pF;
		break;
	}
	assert( punched_face != NULL );
	m_pMesh->deleteFace( punched_face );
}

//filling the small hole

void CPuncture::_fill_hole()
{
/*
	if( m_boundary.loops().size() != 2 )
	{
		fprintf(stderr, "Input mesh should be a topological disk with a small hole\n");
		return;
	}
*/
	CPMesh::CLoop * pL = m_boundary.loops().back();

	std::vector<CPunctureVertex*> verts;

	for( std::list<CHalfEdge*>::iterator hiter = pL->halfedges().begin(); hiter != pL->halfedges().end(); hiter ++ )
	{
		CHalfEdge * pH = *hiter;
		verts.push_back( m_pMesh->halfedgeTarget( pH ) );
	}

	CPunctureVertex *v[3];
	
	for( int i = 0; i  < 3; i ++ )
	{
		v[i] = verts[2-i];
	}


	int fid = -1;

	for( CPMesh::MeshFaceIterator fiter( m_pMesh ); !fiter.end(); fiter ++ )
	{
		CFace * pF = *fiter;
		fid = (fid> pF->id())? fid: pF->id();
	}
	
	m_pMesh->createFace( v, fid + 1 );
}