/* pair lists for MPSA v1.0
 * maintained by g.winter
 * 15th january 2001
 * 
 */

#include "mpsa_export.h"
#include "tcl.h"
#include "pairlist_export.h"

int pairlist_Cmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
)
{
  if(argc < 2) {
    Tcl_AppendResult(interp, argv[0], "takes create/destroy/clear", 
      (char *) NULL);
    return TCL_ERROR;
  }

  if((strcmp(argv[1], "Create") == 0) ||
     (strcmp(argv[1], "create") == 0)) {

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_Create(interp, argv[2]) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Delete") == 0) ||
	    (strcmp(argv[1], "delete") == 0)) {

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_Delete(interp, argv[2]) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Clear") == 0) || 
	    (strcmp(argv[1], "clear") == 0)) {

    pairlist_List *List;

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_GetPairlist(interp, argv[2], &List) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    } else {
      pairlist_Clear(List);
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Sort") == 0) ||
	    (strcmp(argv[1], "sort") == 0)) {

    pairlist_List *List;

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a name", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_GetPairlist(interp, argv[2], &List) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    } else {
      if(List->NElements == 0) {
	Tcl_AppendResult(interp, "List empty", (char *) NULL);
	return TCL_OK;
      }
      pairlist_Order(List);
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Print") == 0) ||
	    (strcmp(argv[1], "print") == 0)) {
    char number[10];
    char PclA[20], PclB[20], rad[20];
    pairlist_Link *Link;
    pairlist_List *List;

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a pairlist", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_GetPairlist(interp, argv[2], &List) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    }

    if(List->NElements == 0) {
      return TCL_OK;
    } else {
      sprintf(number, "%d", List->NElements);
      Tcl_AppendResult(interp, "number in pairlist: ", number, "\n", 
	(char *) NULL);
      for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
	sprintf(PclA, "%d\t", Link->PclA);
	sprintf(PclB, "%d\t", Link->PclB);
	sprintf(rad, "%f\t", Link->index);
	Tcl_AppendResult(interp, rad, PclA, PclB, "\n", (char *) NULL);
      }
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "RemoveDoubles") == 0) ||
	    (strcmp(argv[1], "removedoubles") == 0)) {
    pairlist_List *List;

    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires a pairlist", (char *) NULL);
      return TCL_ERROR;
    }

    if(pairlist_GetPairlist(interp, argv[2], &List) != PAIRLIST_OKAY) {
      return TCL_ERROR;
    }

    pairlist_RemoveDoubles(List);

    return TCL_OK;

  } else {
    Tcl_AppendResult(interp, argv[1], " not recognised", (char *) NULL);
    return TCL_ERROR;
  }

  return TCL_OK;
}
