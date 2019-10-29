#ifndef _API_H_

/*!
 *	Trait IO 
 */
#include "parser/traits_io.h"

/*************************************************************************************************************************
*
*	Conformal Geometric methods
*
*************************************************************************************************************************/


/*! SlitMap
*/
#include "Conformal/SlitMap/SlitMap.h"
/*!	Harmonic Exact Form
 */
#include "Conformal/ExactForm/HarmonicExactForm.h" //harmonic exact form
/*! Holomorphic 1-form
 */
#include "Conformal/HolomorphicForm/HolomorphicForm.h" //holomorphic 1-form
/*!	Compute the Exponential Map : \f$ z\to e^{z}\f$
 */
#include "Conformal/PolarMap/PolarMap.h" //Compute the exponential map z \to e^{z}

/*!	Heat Flow
 */
#include "Conformal/ClosedForm/HeatFlow.h" 


/*************************************************************************************************************************
*
*	Topological methods
*
*************************************************************************************************************************/

/*! Integrating holomorphic 1-form on a simply connected surface
 */
#include "Topology/Integration/Integration.h" //Integrating holomorphic 1-form on a simply connected surface
/*!	Slice the open along sharp edges to form another mesh - Wedge mesh
 */
#include "Topology/Wedge/WMesh.h" //Slice the open along sharp edges to form another mesh - Wedge mesh

/*!	Cohomology Group basis for multiply connected domain, closed one form
 */
#include "Topology/Cohomology/DomainCohomology.h" 
/*! Puncture a hole in the center of the mesh
 */
#include "Topology/puncture/puncture.h" //Puncture a hole in the center of the mesh

/*!	Shortest Path
 */
#include "Riemannian/ShortestPath/ShortestPath.h" //shortest path


/************************************************************************************************************************************
*
*	Conformal Geometric Algorithms
*
************************************************************************************************************************************/



/*!	compute the slit map for topological multi-hole anuli to a planar annulus with concentric circular slits
 *
 */
void _slit_map( int argc, char * argv[] );

/*! compute the basis for all the exact harmonic forms on a surface
 *
 */
void _exact_form( const char * _input, const char * _output );
/*!	compute the basis of all holomorphic 1-forms on a surface
 *
 */
void _holomorphic_form( int argc,  char * argv[] );
/*!	Integrate a holomorphic 1-form on a simply connected surface
 *
 */
void _integration( const char * _form_input, const char * _domain_input, const char *_output );
/*! Compute the exponential map
 *
 */
void _polar_map( const char * _closed_mesh, const char * _open_mesh, const char * _output );
/*!	diffuse the closed one form basis to a harmonic one form
 *
 */
void _diffuse( const char * _closed_1_form, const char * _fundamental_domain, const char * _harmonic_1_form, const char * _harmonic_form_integrated );


/*!	Linear combination of holomorphic 1-forms
*
*/

void _linear_combination( int argc, char * argv[] );


/************************************************************************************************************************************
*
*	Topological Algorithms
*
************************************************************************************************************************************/

/*!	Compute the cut graph of a mesh
 *
 */
void _cut_graph( const char * _input, const char * _output );
/*!	compute the shortest path connecting an inner boundary to the exterior boundary
 *
 */
void _cut_domain( const char * _domain_mesh, const char * _mesh_with_cut );
/*! Slice the mesh open along the sharp edges
 *
 */
void _slice( const char * _closed_mesh, const char * _open_mesh );
/*!	compute the homology group basis
 *
 */
void _homology( const char * _input, const char * _output );
/*!	compute the closed one form basis
 *
 */
void _cohomology_one_form( const char * _closed_mesh, const char * _open_mesh, const char * _closed_1_form, const char * _integrated );
/*!	compute the closed one form basis for multiply connected domain
 *
 */
void _cohomology_one_form_domain( const char * _input_mesh, const char * _sliced_mesh, const char * _closed_1_form, const char * _integrated_mesh );
/*! Puncture a hole in the center of the mesh
 *
 */
void _puncture( const char * _input_mesh, const char * _mesh_with_hole );
/*! Fill the hole in the center
 *
 */
void _fill_puncture( const char * _mesh_with_hole, const char * _filled_mesh );


#endif _API_H_
