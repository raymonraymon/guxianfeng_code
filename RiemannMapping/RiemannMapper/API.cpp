#include "API.h"

using namespace MeshLib;

/*! Puncture a hole in the center of the mesh
 *
 */
void _puncture( const char * _input_mesh, const char * _mesh_with_hole )
{
	CPMesh cmesh;
	cmesh.read_m( _input_mesh );

	CPuncture cut( & cmesh );
	cut._puncture();

	cmesh.write_m( _mesh_with_hole );
}

/*!	compute the shortest path connecting an inner boundary to the exterior boundary
 *
 */
void _cut_domain( const char * _domain_mesh, const char * _mesh_with_cut )
{
	CSPMesh spm;
	spm.read_m( _domain_mesh );

	CShortestPath sp( & spm );
	sp._cut( _mesh_with_cut );
}


/*! Slice the mesh open along the sharp edges
 *
 */
void _slice( const char * _closed_mesh, const char * _open_mesh )
{
	CSMesh mesh;
	mesh.read_m( _closed_mesh );

	CWMesh wmesh( & mesh );
	wmesh.Slice();

	wmesh.wmesh()->write_m( _open_mesh );
};


/*! compute the basis for all the exact harmonic forms on a surface
 *
 */
void _exact_form( const char * _input, const char * _output )
{
	CHarmonicMesh hm;
	hm.read_m( _input );

	CHarmonicExactForm exact_form( & hm );
	exact_form.calculate_harmonic_exact_form( _output );
}


/*!	compute the closed one form basis for multiply connected domain
 *
 */
void _cohomology_one_form_domain( const char * _input_mesh, const char * _sliced_mesh, const char * _closed_1_form, const char * _integrated_mesh )
{
	CCHMesh cmesh;
	cmesh.read_m( _input_mesh );

	CCHMesh omesh;
	omesh.read_m( _sliced_mesh );

	CDomainCohomology closed_form( & cmesh, &omesh );
	closed_form.calculate_closed_form();

	cmesh.write_m( _closed_1_form );
	omesh.write_m( _integrated_mesh );
}


/*!	diffuse the closed one form basis to a harmonic one form
 *
 */
void _diffuse( const char * _closed_1_form, const char * _fundamental_domain, const char * _harmonic_1_form, const char * _harmonic_form_integrated )
{
	CHCFMesh cmesh;
	cmesh.read_m( _closed_1_form );

	CHCFMesh omesh;
	omesh.read_m( _fundamental_domain );

	CHeatFlow flow( & cmesh, &omesh );
	flow.calculate_harmonic_form();

	cmesh.write_m( _harmonic_1_form );
	omesh.write_m( _harmonic_form_integrated );
}

/*!	compute the basis of all holomorphic 1-forms on a surface
 *
 */
void _holomorphic_form( int argc,  char * argv[] )
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


/*!	Compute the slit maps
*
*/
void _slit_map( int argc, char * argv[] )
{

	std::vector<CSMMesh*> meshes;

	printf("meshes size %d\n", meshes.size() );

	for( int i = 2; i < argc-3; i ++ )
	{
		CSMMesh * pMesh = new CSMMesh;
		assert( pMesh );
		pMesh->read_m( argv[i] );
		meshes.push_back( pMesh );
	}

	CSlitMap map( meshes );

	int c1 = atoi( argv[argc-3] );
	int c2 = atoi( argv[argc-2] );

	map._slit_map( c1,c2 );

	meshes[0]->write_m( argv[argc-1] );

	for( size_t i = 0; i < meshes.size(); i ++ )	
	{
		delete meshes[i];
	}

}


/*!	Integrate a holomorphic 1-form on a simply connected surface
 *
 */

void _integration( const char * _form_input, const char * _domain_input, const char *_output )
{
	CIMesh holo_mesh;
	CIMesh fund_mesh;

	holo_mesh.read_m( _form_input   );
	fund_mesh.read_m( _domain_input );

	CIntegration integrator( & holo_mesh, & fund_mesh );
	integrator._integrate();

	fund_mesh.write_m( _output );
}

/*! Compute the exponential map
 *
 */
void _polar_map( const char * _closed_mesh, const char * _open_mesh, const char * _output )
{
	CPMMesh mesh;
	mesh.read_m( _closed_mesh );

	CPMMesh open_mesh;
	open_mesh.read_m( _open_mesh );

	CPolarMap map( &mesh, &open_mesh );
	map._exponential_map();

	mesh.write_m( _output );
}





/**********************************************************************************************************************************************
*
*	Topological Algorithms
*	
*
**********************************************************************************************************************************************/


/*! Fill the hole in the center
 *
 */
void _fill_puncture( const char * _mesh_with_hole, const char * _filled_mesh )
{

	CPMesh cmesh;
	cmesh.read_m( _mesh_with_hole );

	CPuncture cut( & cmesh );
	cut._fill_hole();

	cmesh.write_m( _filled_mesh );
}

