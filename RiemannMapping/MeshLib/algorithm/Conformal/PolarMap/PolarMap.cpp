/*!
*      \file PolarMap.cpp
*      \brief Implement CPolarMap class
*	   \author David Gu
*      \date Documented 10/12/2010
*
*		Compute the exponential map \f$z\to e^{z}\f$
*
*/

#include "PolarMap.h"



using namespace MeshLib;
//CPolarMap constructor
//\param pClosedMesh the output mesh
//\param pOpenMesh   the input mesh with vertex uv

CPolarMap::CPolarMap( CPMMesh * pClosedMesh, CPMMesh * pOpenMesh )
{
	m_pClosedMesh = pClosedMesh;
	m_pOpenMesh   = pOpenMesh;
}
//CPolarMap destructor
CPolarMap::~CPolarMap()
{
}

//Computing the exponential map, 
//Assumption, the range of y value of input vertex uv is from 0 to 1.

void CPolarMap::_exponential_map()
{
	/*
		for(CPMMesh::MeshVertexIterator viter( m_pOpenMesh ); !viter.end(); viter ++ )
		{
			CPolarMapVertex * pV = *viter;
			CPoint2 p = pV->uv();
		}
	*/

	double x_max = -1e+10;
	for( CPMMesh::MeshVertexIterator viter( m_pOpenMesh ); !viter.end(); viter ++ )
	 {
		 CPolarMapVertex * pV = *viter;
		 CPoint2 p = pV->uv();
		 x_max = (x_max > p[0])?x_max:p[0];
	}

	printf("X max is %f\n", x_max );

	for( CPMMesh::MeshVertexIterator viter( m_pOpenMesh ); !viter.end(); viter ++ )
	 {
		 CPolarMapVertex * pV = *viter;
		 CPoint2 p = pV->uv();
		 p[0] -= x_max;
		 pV->uv() = p;
	}

	for( CPMMesh::MeshVertexIterator viter( m_pOpenMesh ); !viter.end(); viter ++ )
	 {
		 CPolarMapVertex * pV = *viter;
		 CPoint2 p = pV->uv();


		double ang = p[1] * 2 * PI;
		double rad = exp( p[0]* 2 * PI );

		 CPoint2 uv( rad * cos(ang), rad * sin(ang ));
		 pV->uv() = uv;

	}

	

	for( CPMMesh::MeshVertexIterator viter( m_pOpenMesh ); !viter.end(); viter ++ )
	{
		CPolarMapVertex * pV = *viter;
		int id = pV->father();
		CPolarMapVertex * pW = m_pClosedMesh->idVertex( id );
		pW->uv() = pV->uv();
	}
	

}