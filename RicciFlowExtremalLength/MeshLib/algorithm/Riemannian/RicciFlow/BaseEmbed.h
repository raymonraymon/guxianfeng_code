/*! \file BaseEmbed.h
 *  \brief Isometrically embed a mesh with canonical metric onto the canonical domain
 *  \author David Gu
 *  \date  documented on 10/17/2010
 *
 *  Isometrically embed a mesh with canonical metric onto the canonical domain
 */
#ifndef  _BASE_EMBED_H_
#define  _BASE_EMBED_H_

#include <vector>
#include <queue>

#include "RicciFlowMesh.h"

namespace MeshLib
{
/*! \brief CBaseEmbed class
 *
 *   embed a mesh with canonical metric onto the canonical domain
 */
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
  class CBaseEmbed
  {
  public: 
   /*! \brief CEmbed constructor */
    CBaseEmbed( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge> * pMesh );
   /*! \brief CEmbed destructor */
    ~CBaseEmbed(){};
	/*! _embed the mesh */
    void _embed();

  protected:
   
	/*! embed the first face 
	 * \param head the first face
	 */
	virtual void _embed_first_face( CFace * head ) = 0;
	/*!
	 *
	 */
	virtual void _embed_face( CFace * head ) = 0;

	/*! initialization */
	void _initialize();

  protected:

	/*! the mesh to be embedded */
    CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge> * m_pMesh;

	/*! queue of faces */
	std::queue<CFace*> m_queue;
  };


//constructor
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
CBaseEmbed<CVertex,CEdge,CFace,CHalfEdge>::CBaseEmbed( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge> * pMesh )
{
    m_pMesh = pMesh;
};




template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CBaseEmbed<CVertex,CEdge,CFace,CHalfEdge>::_initialize()
{
	for( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge>::MeshVertexIterator viter(  m_pMesh ); !viter.end(); viter ++)
  {
    CVertex * v = *viter;
    v->huv() = CPoint2( 0, 0);
    v->touched() = false;
  }

  CFace * root_face = NULL;

  for( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge>::MeshFaceIterator fiter( m_pMesh ); !fiter.end(); fiter ++ )
  {
    CFace * f = *fiter;
    f->touched() = false;
    root_face = f;
  }

	_embed_first_face( root_face );
	root_face->touched() = true;

  CHalfEdge * he = m_pMesh->faceMostCcwHalfEdge( root_face );
  do{
    CHalfEdge  * sh = m_pMesh->halfedgeSym( he );
    if( sh != NULL )
    {
      CFace * df = m_pMesh->halfedgeFace( sh );
      m_queue.push( df );
      df->touched() = true;
    }
    he = m_pMesh->faceNextCcwHalfEdge( he );
  }while ( he != m_pMesh->faceMostCcwHalfEdge( root_face ) );

 };


template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CBaseEmbed<CVertex,CEdge,CFace,CHalfEdge>::_embed()
{

	_initialize();

	int count = 0;

	while( !m_queue.empty() )
	{
		CFace *  head = m_queue.front();
		m_queue.pop();
		assert( head->touched() );

		CHalfEdge * he = m_pMesh->faceMostCcwHalfEdge( head );
		do{
		  CHalfEdge * sh = m_pMesh->halfedgeSym( he );
		  if( sh != NULL )
		  {  
			CFace * df = m_pMesh->halfedgeFace( sh );
			if( !df->touched() )
			{
				m_queue.push( df );
				df->touched() = true;
			}
		  }
		  he = m_pMesh->halfedgeNext( he );
		}while( he != m_pMesh->faceMostCcwHalfEdge( head ) );

		_embed_face( head );
	}

	//normalize the uv coordinates

	double u_min, u_max, v_min, v_max;
	u_min = v_min = 1e30;
	u_max = v_max = -1e30;


	for( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter++ )
	{
		CVertex * pV = *viter;
		CPoint2 uv = pV->huv();
		double u_param = uv[0];
		double v_param = uv[1];
		if( u_min>u_param )
			u_min = u_param;
		if( u_max<u_param )
			u_max = u_param;
		if( v_min>v_param )
			v_min = v_param;
		if( v_max<v_param )
			v_max = v_param;
	}

	double range = ( u_max-u_min )>( v_max-v_min ) ? u_max-u_min : v_max-v_min;

	printf( "range = %lf\n", range );

	if( range>1e-6 ) {
		for( CRicciFlowMesh<CVertex,CEdge,CFace,CHalfEdge>::MeshVertexIterator viter( m_pMesh ); !viter.end(); viter++ )
		{
			CVertex * pV = *viter;
			pV->u() -= log( range );
			CPoint2 uv =  pV->huv();
			double u_param = uv[0];
			double v_param = uv[1];
			uv[0] = (u_param - u_min)/range;
			uv[1] = (v_param - v_min)/range;
			pV->huv() = uv;
		}
	}
};

}


#endif 