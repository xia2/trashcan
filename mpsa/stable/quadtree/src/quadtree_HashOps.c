/* quadtree module hash operations file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 */

#include "quadtree_private.h"

/*[ quadtree_CreateQuadtree
 *[ action:  creates a new 'blank' quadtree trunk and pops it into a hash
 *[          table
 *[ objects: takes an interpreter and a name for the quadtree
 */

int quadtree_CreateQuadtree(
  Tcl_Interp *interp,
  char *Name
)
{
  Tcl_HashEntry *Entry;
  int new, i;
  quadtree_Node *Root;
  Root = (quadtree_Node *) malloc (sizeof(quadtree_Node));
  Root->Leaf = NULL;
  Root->Trunk = NULL;
  Root->Branch = NULL;
  Root->size = 1;
  for(i = 0; i < 3; i++) {
    Root->centre[i] = 0;
  }

  Entry = Tcl_CreateHashEntry(&quadtree_QuadtreeHashTable, Name, &new);
  if(new) {
    Tcl_SetHashValue(Entry, Root);
  } else {
    free(Root);
    Tcl_AppendResult(interp, "Error setting hash table entry", (char *) NULL);
    return QUADTREE_FAIL;
  }

  return QUADTREE_OKAY;
}

/*[ quadtree_GetQuadtree
 *[ action:  retrieves a quadtree pointer from a hash table
 *[ objects: takes an interpreter, name of a quadtree and returns a pointer
 */

int quadtree_GetQuadtree(
  Tcl_Interp *interp,
  char *Name,
  quadtree_Node **Root
)
{
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&quadtree_QuadtreeHashTable, Name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No quadtree of that name in the hash table", 
      (char *) NULL);
    return QUADTREE_FAIL;
  } else {
    *Root = (quadtree_Node *) Tcl_GetHashValue(Entry);
  }

  return QUADTREE_OKAY;
}

/*[ quadtree_DeleteQuadtree
 *[ action:  removes the hash table entry and data from a quadtree
 *[ objects: takes a name
 */

int quadtree_DeleteQuadtree(
  Tcl_Interp *interp,
  char *Name
)
{
  quadtree_Node *Node;
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&quadtree_QuadtreeHashTable, Name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No quadtree of that name in the hash table", 
      (char *) NULL);
    return QUADTREE_FAIL;
  } else {
    Node = (quadtree_Node *) Tcl_GetHashValue(Entry);
  }

  quadtree_CloseNode(Node);
  Tcl_DeleteHashEntry(Entry);
  free(Node);
  return QUADTREE_OKAY;
}
