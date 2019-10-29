/*!
*      \file DomainCohomology.cpp
*      \brief Implement CDomainCohomology class
*	   \author David Gu
*      \date Document 10/11/2010
*
*		Compute cohomology group for multiply connected domains
*
*/
#include <queue>
#include "DomainCohomology.h"

using namespace MeshLib;


//CDomainCohomology constructor
//pMesh is the input closed mesh
//pWMesh is the open mesh, obtained by slicing the closed mesh along speical paths
CDomainCohomology::CDomainCohomology( CCHMesh * pMesh, CCHMesh * pWMesh )
{
	m_pMesh  = pMesh; 
	m_pWMesh = pWMesh;
};

//CDomainCohomology destructor
CDomainCohomology::~CDomainCohomology()
{
};


//Compute the cohomology group basis for multiply connected domains

void CDomainCohomology::calculate_closed_form()
{
	//compute the closed 1-forms
    _closed_1_form();
	//integrate the harmnoic 1-forms on the mesh
    _integrate();
}





//integrate the closed 1-form on the mesh 

void CDomainCohomology::_integrate()
{
	for( CCHMesh::MeshEdgeIterator eiter( m_pWMesh ); !eiter.end(); eiter ++ )
  {
      CCHEdge * we = *eiter;

      CCHVertex * w1 = m_pWMesh->edgeVertex1( we );
      CCHVertex * w2 = m_pWMesh->edgeVertex2( we );

      int id1 = w1->father();
      int id2 = w2->father();

      CCHVertex * v1 = m_pMesh->idVertex( id1 );
      CCHVertex * v2 = m_pMesh->idVertex( id2 );

      CCHEdge * e = m_pMesh->vertexEdge( v1, v2 );

      if( m_pMesh->edgeVertex1( e ) == v1 )
      {
          we->du()  = e->du();
      }
      else
      {  
        we->du() = - e->du();
      }
  }

  CCHVertex * head = NULL;
  for( CCHMesh::MeshVertexIterator viter( m_pWMesh ); !viter.end(); viter ++ )
  {
      CCHVertex * v = *viter;
      v->touched() = false;
      head = v;
  }

  std::queue<CCHVertex*> vqueue;
  head->touched() = true;
  vqueue.push( head );
  head->uv() =  CPoint2(0,0.53);

  while( !vqueue.empty() )
  {
    CCHVertex * head = vqueue.front();
    vqueue.pop();

	for( CCHMesh::VertexEdgeIterator veiter(  head ); !veiter.end(); veiter ++ )
    {
      CCHEdge * e = *veiter;
      double du = e->du();

      CCHVertex * v1 = m_pWMesh->edgeVertex1( e );
      CCHVertex * v2 = m_pWMesh->edgeVertex2( e );

      if( v1 == head )
      {
        if( v2->touched() ) continue;
        v2->touched( ) = true;
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
 /* 
  double umin = 1e+10;
  double umax = -1e+10;

  for( CHCFMesh::MeshVertexIterator viter( m_pWMesh );  !viter.end(); viter ++ )
  {
      CHCFVertex* v = *viter;
  	  CPoint2 p = v->uv()
      umax = (umax > p[0] )? umax: p[0];
      umin = (umin < p[0] )? umin: p[0];
  }

 for( MeshVertexIterator viter( m_pWMesh );  !viter.end(); viter ++ )
  {
      CVertex* v = *viter;
  	  CPoint2 p = v_harmonic_closed_form_uv( v );
      p[0] = ( p[0] - umin )/(umax-umin);
      v_harmonic_closed_form_uv( v ) = p;
  }
*/

}


//Compute the closed 1-form

void CDomainCohomology::_closed_1_form()
{
  for( CCHMesh::MeshVertexIterator viter( m_pWMesh ); !viter.end(); viter ++ )
  {
      CCHVertex * pV = *viter;
      pV->u() = 0;
  }
 
  for( CCHMesh::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter ++ )
  {
      CCHVertex * pV = *viter;
      pV->valence() = 0;
  }

  for( CCHMesh::MeshVertexIterator viter( m_pWMesh ); !viter.end(); viter ++ )
  {
      CCHVertex * pV = *viter;
      int fid = pV->father();
	  CCHVertex * w = m_pMesh->idVertex( fid );
	  w->valence() ++;
  }
  
  
  _label_closed_mesh_boundary_loop_id();

  std::vector<CCHVertex*>  corners;
  _locate_four_corners(  corners );

  //the vertices between corner[1] and corner[2];
  //  2---- 1
  //  |     |
  //  |     |
  //  3---- 0

   std::vector<CCHVertex*> side;
	
    CCHVertex   * pV = corners[1];
	side.push_back( pV );
	CCHVertex   * pN = pV;

	do{
		CHalfEdge * pH = m_pWMesh->vertexMostClwOutHalfEdge( pN );
		pN = m_pWMesh->halfedgeVertex( pH );
		side.push_back( pN );
	}while( pN != corners[2] );


 for( int k = 0; k < (int)side.size(); k ++ )
 {
	 CCHVertex * v = side[k];
	 v->u() = 1.0;
 }

 for( CCHMesh::MeshEdgeIterator eiter( m_pWMesh ); !eiter.end(); eiter ++ )
  {
    CCHEdge   * we = *eiter;
    CCHVertex * w1 = m_pWMesh->edgeVertex1( we );
    CCHVertex * w2 = m_pWMesh->edgeVertex2( we );
	we->du() = w2->u() - w1->u();

    int id1 =  w1->father();
    int id2 =  w2->father();

    CCHVertex * v1 = m_pMesh->idVertex( id1 );
    CCHVertex * v2 = m_pMesh->idVertex( id2 );

    CCHEdge * e = m_pMesh->vertexEdge( v1, v2 );

    if( m_pMesh->edgeVertex1( e ) == v1 )
    {
        e->du() = we->du();
    }
    else
    {
		assert( m_pMesh->edgeVertex1( e ) == v2 );
        e->du() = -we->du();
    }
  }
}



  //locate 4 corner vertices
  //the vertices between corner[1] and corner[2];
  //  2---- 1
  //  |     |
  //  |     |
  //  3---- 0

void CDomainCohomology::_locate_four_corners(  std::vector<CCHVertex*> & corners )
{

	CCHMesh::CBoundary boundary( m_pWMesh );
  //the longest one	
	CCHMesh::CLoop * pL = boundary.loops().front();

  std::list<CHalfEdge*> pHs = pL->halfedges();
 
  for( std::list<CHalfEdge*>::iterator hiter = pHs.begin(); hiter != pHs.end(); hiter ++)
  {
	  CHalfEdge * pH = *hiter;
	  CCHVertex * pV = m_pWMesh->halfedgeVertex( pH );

	  int fid = pV->father();
	  CCHVertex * w = m_pMesh->idVertex( fid );

	  if( !w->boundary()  )		continue;
	  if( w->valence() != 2 )   continue;
	  corners.push_back( pV );
  }

  //cyclicly ajust the order of the vertices in the cut_verts buffer
  //such that the father of the first 2 corners are on the external boundary loop

  assert( corners.size() == 4 );

  std::vector<CCHVertex*> buffer;

  for( int i = 0; i < 4; i ++ )
  {
	  int fid = corners[i]->father();
	  CCHVertex * w = m_pMesh->idVertex( fid );

	  printf("vertex id %d, father id %d, father loop id %d\n", corners[i]->id(), corners[i]->father(),  w->idx() );
  }

  int min_loop_id = 10000;

  for( int i = 0; i < 4; i ++ )
  {
	  CCHVertex * pV = corners[i];
	  CCHVertex * pW  = m_pMesh->idVertex( pV->father() );

	  min_loop_id = ( min_loop_id < pW->idx() )?min_loop_id: pW->idx();
  }

  for( int i = 0; i < 4; i ++ )
  {
	  CCHVertex * pV = corners[(i+0)%4];
	  CCHVertex * pN = corners[(i+1)%4];

	  CCHVertex * pW  = m_pMesh->idVertex( pV->father() );
	  CCHVertex * pWN = m_pMesh->idVertex( pN->father() );

	  if( pW->idx()  == min_loop_id && pWN->idx() == min_loop_id )
	  {
		for( int j = 0; j < 4; j ++ )
		{
			buffer.push_back( corners[(i+j)%4] );
		}
	  }
  }

  corners.clear();

  for( int i = 0; i < 4; i ++ )
  {
	  corners.push_back( buffer[i] );
  }

  for( int i = 0; i < 4; i ++ )
  {
	  int fid = corners[i]->father();
	  CCHVertex * w = m_pMesh->idVertex( fid );

	  printf("vertex id %d, father id %d, father loop id %d\n", corners[i]->id(), corners[i]->father(),  w->idx() );
  }

};

//label closed mesh boundary ids

void CDomainCohomology::_label_closed_mesh_boundary_loop_id()
{
  //sort boundary loops by their lengths
  
	CCHMesh::CBoundary boundary( m_pMesh );

	std::vector<CCHMesh::CLoop*>& loops = boundary.loops();

	
  for( size_t k = 0; k < loops.size(); k ++ )
  {
	  CCHMesh::CLoop * pL = loops[k];
	  std::list<CHalfEdge*>  & pH = pL->halfedges();
    
	for( std::list<CHalfEdge*>::iterator hiter = pH.begin(); hiter != pH.end(); hiter ++ )
    {
        CHalfEdge * he = *hiter;
		CCHVertex * pV = m_pMesh->halfedgeVertex( he );
        pV->idx() = k;
    }
 }

};


