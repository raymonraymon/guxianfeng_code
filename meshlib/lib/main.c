#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"
#include "funcs.h"

Solid * solid;

void main(int argc, char * argv[] ){


SolidConstruct(&solid, argv[1]);
FaceListOutput(solid->sfaces);
//SolidDestruct( &solid );


}
