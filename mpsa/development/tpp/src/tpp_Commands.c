#include "tpp_export.h"

/*[ tpp_TppCmd
 *[ action:  anything to do with tpps
 *[ objects: probably takes tpps and lists
 *[ syntax:  Tpp ...stuff...
 */

int tpp_TppCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
)
{
  if(argc < 2) {
    Tcl_AppendResult(interp, "Expecting one of create/delete/load/clear/setup",
      (char *) NULL);
    return TCL_ERROR;
  }

  if((strcmp(argv[1], "Create") == 0) ||
     (strcmp(argv[1], "create") == 0)) {
    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires name of a tpp to make", 
        (char *) NULL);
      return TCL_ERROR;
    }
    if(tpp_CreateTpp(interp, argv[2]) != TPP_OKAY) {
      return TCL_ERROR;
    }

    return TCL_OK;
  } else if((strcmp(argv[1], "Delete") == 0) ||
	    (strcmp(argv[1], "delete") == 0)) {
    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires name of a tpp to delete", 
	(char *) NULL);
      return TCL_ERROR;
    }
    if(tpp_DeleteTpp(interp, argv[2]) != TPP_OKAY) {
      return TCL_ERROR;
    }
    return TCL_OK;
  } else if((strcmp(argv[1], "Load") == 0) ||
	    (strcmp(argv[1], "load") == 0)) {
    mpsa_List *List;
    tpp_Node *Node;

    if(argc < 4) {
      Tcl_AppendResult(interp, argv[1], "requires a tpp and a list", 
        (char *) NULL);
      return TCL_ERROR;
    }

    if(tpp_GetTpp(interp, argv[2], &Node) != TPP_OKAY) {
      return TCL_ERROR;
    }

    if(mpsa_GetList(interp, argv[3], &List) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    if(List->NElements == 0) {
      return TCL_OK;
    }

    if(tpp_LoadList(Node, List) != TPP_OKAY) {
      Tcl_AppendResult(interp, "Error loading list", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;
  } else if((strcmp(argv[1], "Clear") == 0) ||
	    (strcmp(argv[1], "clear") == 0)) {
    tpp_Node *Node;

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a tpp", (char * ) NULL);
      return TCL_ERROR;
    }
  
    if(tpp_GetTpp(interp, argv[2], &Node) != TPP_OKAY) {
      return TCL_ERROR;
    }

    if(tpp_CloseNode(Node) != TPP_OKAY) {
      Tcl_AppendResult(interp, "Error clearing tpp", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Print") == 0) ||
	    (strcmp(argv[1], "print") == 0)) {
    tpp_Node *Node;

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a tpp", (char * ) NULL);
      return TCL_ERROR;
    }
  
    if(tpp_GetTpp(interp, argv[2], &Node) != TPP_OKAY) {
      return TCL_ERROR;
    }

    tpp_PrintTPP(Node, interp);

    return TCL_OK;

  } else {
    Tcl_AppendResult(interp, "Option not recognised", (char *) NULL);
    return TCL_ERROR;
  }

  return TCL_OK;
}
