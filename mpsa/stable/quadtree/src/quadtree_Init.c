/* quadtree module initialisation file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 */

#include "quadtree_private.h"

/*
 * hash tables to hold dynamically created quadtrees
 */

Tcl_HashTable quadtree_QuadtreeHashTable;

int Quadtree_Init(
  Tcl_Interp *interp
)
{
  Tcl_InitHashTable(&quadtree_QuadtreeHashTable, TCL_STRING_KEYS);

  Tcl_CreateCommand(interp, "::quadtree::quadtree", quadtree_QuadtreeCmd,
    (ClientData) NULL, (void(*)()) NULL);

  return TCL_OK;
}
