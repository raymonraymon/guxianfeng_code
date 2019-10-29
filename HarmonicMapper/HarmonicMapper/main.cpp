/*!
*      \file main.cpp
*      \brief Main program for Slit Mapping/Riemann Mapping/Harmonic Mapping
*	   \author David Gu
*      \date Document 10/07/2010
*
*		The code performs the following tasks
*
*       1. Compute the harmonic maps for simply connected domain with a single boundary component to the unit disk.
*/

/*!	Harmonic Mapping
 */
#include "Conformal/HarmonicMapper/HarmonicMapperMesh.h" //Harmonic Mapping
#include "Conformal/HarmonicMapper/HarmonicMapper.h" //Harmonic Mapping

using namespace MeshLib;


/*!	\brief Helper function to remind the users of the usage of the commands	
 *
 */

void help(char * exe )
{
	printf("Usage:\n");
	printf("%s -harmonic_map  input_mesh output_mesh_with_uv\n", exe );
}

//compute harmonic map, between a topological disk to a disk
void _harmonic_map( const char * _input, const char * _output )
{
	CHMMesh mesh;
	mesh.read_m( _input );

	CHarmonicMapper mapper( & mesh );
	mapper._map();
	mesh.write_m( _output );
};




/*!	\brief main function to call all the functionalities
 * 
 */

int main( int argc, char * argv[] )
{

	if( strcmp( argv[1] , "-harmonic_map") == 0 && argc == 4 )
	{
		_harmonic_map( argv[2], argv[3] );
		return 0;
	}

	help( argv[0] );
	return 0;
}
 