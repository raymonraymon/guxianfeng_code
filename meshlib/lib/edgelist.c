#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mesh.h"
#include "funcs.h"

void  EdgeListConstruct(Solid ** solid )
{
   Face * fhead, *tf;
   HalfEdge *the, *he_mate;

   fhead = (*solid)->sfaces;
   assert( fhead);
   tf = fhead;
   do{
   
   the = tf->floop->ledges;
   do{

     if( the->hedge == NIL ){
     
       he_mate = HalfEdgeMate(the);
       EdgeConstruct( solid, the, he_mate);
     }

   the = the->next;

   }while( the != tf->floop->ledges );   


   tf = tf->next;
   }while( tf != fhead );

}


void  EdgeListDestruct(Solid ** solid )
{
	 Edge * te;


	 while( (*solid)->sedges ){
	 te = (*solid)->sedges;
	 assert( te->he1 == NIL );
	 assert( te->he2 == NIL );
	 EdgeDestruct(&te);
	 }

}



