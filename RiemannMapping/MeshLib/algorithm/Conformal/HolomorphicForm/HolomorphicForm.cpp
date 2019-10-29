/*! \file HolomorphicForm.cpp
*   \brief Implementation of HolomorphicForm class
*
*   Computing Hodge Star operator and holomorphic 1-form basis on the mesh.
*   
*/
#include "HolomorphicForm.h"

using namespace MeshLib;



//CHolomorphicForm constructor
//\param meshes are the basis of harmonic 1-form group

CHolomorphicForm::CHolomorphicForm( std::list<CHoloFormMesh*> & meshes ) :CBaseHolomorphicForm( meshes )
{
};

//CHolomorphicForm destructor

CHolomorphicForm::~CHolomorphicForm()
{
};

//Compute the angle structure
void CHolomorphicForm::_angle_structure()
{
	for( std::vector<CHoloFormMesh*>::iterator miter = m_meshes.begin(); miter != m_meshes.end(); miter ++ )
	{
		CHoloFormMesh * pM = *miter;

		CStructure<CHoloFormMesh, CVertex, CHoloFormEdge, CHoloFormFace, CHoloFormHalfEdge> pC( pM );
		pC._embedding_2_metric();
		pC._metric_2_angle();
	}

}
