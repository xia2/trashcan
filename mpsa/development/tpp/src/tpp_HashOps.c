#include "tpp_export.h"

/*[ tpp_CreateTpp
 *[ action:  creates a new 'blank' tpp trunk and pops it into a hash
 *[          table
 *[ objects: takes an interpreter and a name for the tpp
 */

int tpp_CreateTpp(
  Tcl_Interp *interp,
  char *Name
)
{
  Tcl_HashEntry *Entry;
  int new, i;
  tpp_Node *Root;
  Root = (tpp_Node *) malloc (sizeof(tpp_Node));
  Root->Leaf = NULL;
  Root->Branch = NULL;
  Root->size_x = 1;
  Root->size_v = 1;
  for(i = 0; i < 3; i++) {
    Root->centre_x[i] = 0;
    Root->centre_v[i] = 0;
  }

  Entry = Tcl_CreateHashEntry(&tpp_TppHashTable, Name, &new);
  if(new) {
    Tcl_SetHashValue(Entry, Root);
  } else {
    free(Root);
    Tcl_AppendResult(interp, "Error setting hash table entry", (char *) NULL);
    return TPP_FAIL;
  }

  return TPP_OKAY;
}

/*[ tpp_GetTpp
 *[ action:  retrieves a tpp pointer from a hash table
 *[ objects: takes an interpreter, name of a tpp and returns a pointer
 */

int tpp_GetTpp(
  Tcl_Interp *interp,
  char *Name,
  tpp_Node **Root
)
{
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&tpp_TppHashTable, Name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No tpp of that name in the hash table", 
      (char *) NULL);
    return TPP_FAIL;
  } else {
    *Root = (tpp_Node *) Tcl_GetHashValue(Entry);
  }

  return TPP_OKAY;
}

/*[ tpp_DeleteTpp
 *[ action:  removes the hash table entry and data from a tpp
 *[ objects: takes a name
 */

int tpp_DeleteTpp(
  Tcl_Interp *interp,
  char *Name
)
{
  tpp_Node *Node;
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&tpp_TppHashTable, Name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No tpp of that name in the hash table", 
      (char *) NULL);
    return TPP_FAIL;
  } else {
    Node = (tpp_Node *) Tcl_GetHashValue(Entry);
  }

  tpp_CloseNode(Node);
  Tcl_DeleteHashEntry(Entry);
  free(Node);
  return TPP_OKAY;
}
