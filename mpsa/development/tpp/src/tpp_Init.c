#include "tpp_export.h"

/*
 * hash tables to hold dynamically created tpps
 */

Tcl_HashTable tpp_TppHashTable;

int Tpp_Init(
  Tcl_Interp *interp
)
{
  Tcl_InitHashTable(&tpp_TppHashTable, TCL_STRING_KEYS);

  Tcl_CreateCommand(interp, "::tpp::tpp", tpp_TppCmd,
    (ClientData) NULL, (void(*)()) NULL);

  return TCL_OK;
}
