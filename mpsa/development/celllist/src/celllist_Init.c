#include "celllist_export.h"

Tcl_HashTable celllist_HashTable;

int Celllist_Init(
  Tcl_Interp *interp
)
{
  Tcl_InitHashTable(&celllist_HashTable, TCL_STRING_KEYS);

  Tcl_CreateCommand(interp, "::celllist::celllist", celllist_celllistCmd,
    (ClientData) NULL, (void(*)()) NULL);

  return TCL_OK;
}
