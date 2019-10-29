/*!
*      \file BaseHolomorphicForm.h
*      \brief Algorithm for computing holomorphic 1-forms, Hodge star operator
*	   \author David Gu
*      \date Document 10/12/2010
*
*		Algorithm for computing holomorphic 1-forms, Hodge Star operator
*
*/
/********************************************************************************************************************************
*
*      BaseHolomorphic 1-form Class
*
*       Copyright (c) Stony Brook University
*
*    Purpose:
*
*       Compute the holomorphic 1-forms
* 
*       David Gu June 27, 2008,  gu@cs.stonybrook.edu
*
*
*      Input:
*         
*           Original mesh, the mesh cut through a shortest path connecting one inner boundary and the exterior boundary
*
*      Output:
*
*           Closed non-exact Harmonic 1-form. and the mesh with UV coordinates.
*
*********************************************************************************************************************************/

/*---------------------------------------------------------------------------------------------------------------------------------
#include "HolomorphicForm/HolomorphicForm.h"

using namespace MeshLib;

int main( int argc, char * argv[] )
{
	if( strcmp( argv[1], "-holomorphic_form" ) == 0 )
	{
	std::list<CHoloFormMesh*> meshes;


	for( int i = 2; i < argc; i ++ )
	{
		CHoloFormMesh * pMesh = new CHoloFormMesh;
		assert( pMesh );
		pMesh->read_m( argv[i] );
		meshes.push_back( pMesh );
	}

	CHolomorphicForm form( meshes );
	form.conjugate();

	int id = 0;
	for( std::list<CHoloFormMesh*>::iterator miter = meshes.begin(); miter != meshes.end(); miter++)
	{
		CHoloFormMesh * pM = *miter;
		std::stringstream iss;
		iss << "holomorphic_form_" << id++ << ".m";
		pM->write_m( iss.str().c_str() );
	}

	for( std::list<CHoloFormMesh*>::iterator miter = meshes.begin(); miter != meshes.end(); miter++)
	{
		CHoloFormMesh * pM = *miter;
		delete pM;
	}
	
}
----------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _BASE_HOLOMORPHIC_FORM_H_
#define _BASE_HOLOMORPHIC_FORM_H_

#include  <math.h>
#include <queue>
#include <list>
#include <vector>
#include "HolomorphicFormMesh.h"
#include "Structure/Structure.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>


namespace MeshLib
{

/*! \brief CWedgeOperator class
 *
 *	Wedge Star Operator
 */

class CWedgeOperator  
{
public:
	/*! CWedgeOperator constructor
	 * \param solid0 harmonic 1-form \f$\omega_0\f$
	 * \param solid1 harmonic 1-form \f$\omega_1\f$
	 */
	CWedgeOperator(CHoloFormMesh * mesh0, CHoloFormMesh * mesh1);
	/*! CWedgeOperator destructor
	*/
	~CWedgeOperator();
	/*! wedge product 
	 * \return 
	 *	\f[
				\int \omega_0 \wedge \omega_1
        \f]
	 */
	double wedge_product();
	/*! wedge product 
	 * \return 
	 *	\f[
				\int \omega_0 \wedge {}^*\omega_1
        \f]
	 */
	double wedge_star_product();
private:
	/*! Two input harmonic 1-forms $\f\omega_0,\omega_1\f$. */
	CHoloFormMesh * m_pMesh[2];
};

/*! \brief CHolomorphicForm class
 *
 *	Compute holomorphic forms on a mesh
 */
class CBaseHolomorphicForm
{
public:
	/*! CHolomorphicForm constructor
	*	\param meshes the list of meshes with stores the harmonic 1-forms, which form the basis 
	*   of the first cohomology group of the mesh
	*/
	CBaseHolomorphicForm( std::list<CHoloFormMesh*> & meshes );
	/*! CHolomorphicForm destructor
	*/
	~CBaseHolomorphicForm();
	/*!	The list of meshes storing harmonic form bases
	*/
	std::vector<CHoloFormMesh*> & meshes() { return m_meshes; };

	/*! Compute the conjugate harmonic 1-forms for the base harmonic 1-forms by solving the equation,
		Assume \f$ {}^*\omega_i = \sum_j \lambda_{ij} \omega_j \f$, then
		\f[
			\int_M \omega_k \wedge {}^*\omega_i = \sum_j \lambda_{ij} \int_M \omega_k \wedge \omega_j
		\f]
		the left hand side can be computed using face-vector represenation.
	*/
	void conjugate();

protected:
	/*!	The list of meshes storing harmonic form bases
	*/
	std::vector<CHoloFormMesh*> m_meshes;
	/*! Convert harmonic 1-form from edge-function to face-vector representation 
	 */
	//void convert( CHoloFormMesh * pMesh, CHoloFormFace * f );

	virtual void _angle_structure() = 0;
};

}
#endif