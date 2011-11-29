/* pair lists for MPSA v1.0
 * maintained by g.winter
 * 15th january 2001
 * 
 * 
 */

#include "tcl.h"
#include "pairlist_export.h"

Tcl_HashTable pairlist_HashTable;

extern int pairlist_Cmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
);


int Pairlist_Init(
  Tcl_Interp *interp
)
{
  Tcl_InitHashTable(&pairlist_HashTable, TCL_STRING_KEYS);

  Tcl_CreateCommand(interp, "::pairlist::pairlist", pairlist_Cmd,
    (ClientData) NULL, (void(*)()) NULL);

  return TCL_OK;
}
