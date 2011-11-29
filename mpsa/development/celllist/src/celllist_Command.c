/* celllist implementation for MPSA
 * 25th january 2001
 * maintained by g.winter
 * 
 */

#include "mpsa_export.h"
#include "celllist_export.h"
#include "tcl.h"

int celllist_celllistCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
)
{
  if(argc < 2) {
    Tcl_AppendResult(interp, argv[0], " takes create/destroy etc", 
      (char *) NULL);
    return TCL_ERROR;
  }

  if((strcmp(argv[1], "Create") == 0) ||
     (strcmp(argv[1], "create") == 0)) {

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(celllist_CreateCelllist(interp, argv[2]) != CELLLIST_OKAY) {
      return TCL_ERROR;
    } else {
      return TCL_OK;
    }

  } else if((strcmp(argv[1], "Delete") == 0) ||
	    (strcmp(argv[1], "delete") == 0)) {

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(celllist_DeleteCelllist(interp, argv[2]) != CELLLIST_OKAY) {
      return TCL_ERROR;
    } else {
      return TCL_OK;
    }

  } else if((strcmp(argv[1], "Clear") == 0) ||
	    (strcmp(argv[1], "clear") == 0)) {

    celllist_List *List;

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a celllist", (char *) NULL);
      return TCL_ERROR;
    }

    if(celllist_GetCelllist(interp, argv[2], &List) != CELLLIST_OKAY) {
      return TCL_ERROR;
    } 

    celllist_ClearCelllist(List);

    return TCL_OK;

  } else if((strcmp(argv[1], "Load") == 0) ||
	    (strcmp(argv[1], "load") == 0)) {
    mpsa_List *List;
    celllist_List *Celllist;
    float dt;

    if(argc < 5) {
      Tcl_AppendResult(interp, argv[1], " requires a list, celllist and dt", 
        (char *) NULL);
      return TCL_ERROR;
    }

    if(mpsa_GetList(interp, argv[2], &List) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    if(celllist_GetCelllist(interp, argv[3], &Celllist) != CELLLIST_OKAY) {
      return TCL_ERROR;
    } 

    if(mpsa_GetFloat(interp, argv[4], &dt) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    if(celllist_SetGridSize(Celllist, List, dt) != CELLLIST_OKAY) {
      Tcl_AppendResult(interp, "Setting grid size failed", (char *) NULL);
      return TCL_ERROR;
    }

    if(celllist_LoadList(Celllist, List) != CELLLIST_OKAY) {
      Tcl_AppendResult(interp, "Loading grid failed", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;
  }

  return TCL_OK;
}
