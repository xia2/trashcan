/* quadtree module command file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 * 
 */

#include "quadtree_private.h"

/*[ quadtree_QuadtreeCmd
 *[ action:  anything to do with quadtrees
 *[ objects: probably takes quadtrees and lists
 *[ syntax:  Quadtree ...stuff...
 */

int quadtree_QuadtreeCmd(
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
      Tcl_AppendResult(interp, argv[1], " requires name of a quadtree to make", 
        (char *) NULL);
      return TCL_ERROR;
    }
    if(quadtree_CreateQuadtree(interp, argv[2]) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }

    return TCL_OK;
  } else if((strcmp(argv[1], "Delete") == 0) ||
	    (strcmp(argv[1], "delete") == 0)) {
    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires name of a quadtree to delete", 
	(char *) NULL);
      return TCL_ERROR;
    }
    if(quadtree_DeleteQuadtree(interp, argv[2]) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }
    return TCL_OK;
  } else if((strcmp(argv[1], "Load") == 0) ||
	    (strcmp(argv[1], "load") == 0)) {
    mpsa_List *List;
    quadtree_Node *Node;

    if(argc < 4) {
      Tcl_AppendResult(interp, argv[1], "requires a quadtree and a list", 
        (char *) NULL);
      return TCL_ERROR;
    }

    if(quadtree_GetQuadtree(interp, argv[2], &Node) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }

    if(mpsa_GetList(interp, argv[3], &List) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    if(quadtree_LoadList(Node, List) != QUADTREE_OKAY) {
      Tcl_AppendResult(interp, "Error loading list", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;
  } else if((strcmp(argv[1], "Clear") == 0) ||
	    (strcmp(argv[1], "clear") == 0)) {
    quadtree_Node *Node;

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a quadtree", (char * ) NULL);
      return TCL_ERROR;
    }
  
    if(quadtree_GetQuadtree(interp, argv[2], &Node) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }

    if(quadtree_CloseNode(Node) != QUADTREE_OKAY) {
      Tcl_AppendResult(interp, "Error clearing quadtree", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;
  } else if((strcmp(argv[1], "CalcCOM") == 0) ||
	    (strcmp(argv[1], "calccom") == 0)) {
    quadtree_Node *Node;
    char result_mass[15];

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " needs a quadtree", (char *) NULL);
      return TCL_ERROR;
    }

    if(quadtree_GetQuadtree(interp, argv[2], &Node) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }

    if(quadtree_CalcCOM(Node) != QUADTREE_OKAY) {
      Tcl_AppendResult(interp, "Error calculating centre of mass of quadtree", 
	(char *) NULL);
      return TCL_ERROR;
    }

    sprintf(result_mass, "%f", Node->mass);
    Tcl_AppendResult(interp, result_mass, (char *) NULL);

    return TCL_OK;

  } else if((strcmp(argv[1], "CalcGrav") == 0) || 
	    (strcmp(argv[1], "calcgrav") == 0)) {
    quadtree_Node *Node;
    mpsa_List *List;
    mpsa_Link *Link;

    if(argc < 4) {
      Tcl_AppendResult(interp, argv[1], " needs a quadtree and a list", 
        (char *) NULL);
      return TCL_ERROR;
    }

    if(quadtree_GetQuadtree(interp, argv[2], &Node) != QUADTREE_OKAY) {
      return TCL_ERROR;
    }

    if(mpsa_GetList(interp, argv[3], &List) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    for(Link = List->firstLink; Link != NULL; Link = Link->nextLink){
      if(quadtree_CalcGrav(Link->Pcl, Node) != QUADTREE_OKAY) {
	Tcl_AppendResult(interp, "Error calculating gravitational force", 
          (char *) NULL);
	return TCL_ERROR;
      }
    }

    return TCL_OK;

  } else if((strcmp(argv[1], "Setup") == 0) || 
	    (strcmp(argv[1], "setup") == 0)) {
    float theta, epsilon;
    if(argc != 4) {
      Tcl_AppendResult(interp, argv[1], " requires theta and epsilon",
        (char *) NULL);
      return TCL_ERROR;
    }

    if(mpsa_GetFloat(interp, argv[2], &theta) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    if(mpsa_GetFloat(interp, argv[3], &epsilon) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    quadtree_SetGravParam(theta, epsilon);

    return TCL_OK;

  } else if((strcmp(argv[1], "DirectGrav") == 0) ||
	    (strcmp(argv[1], "directgrav") == 0)) {
    mpsa_List *List;
    mpsa_Link *Link;

    if(argc < 3) {
      Tcl_AppendResult(interp, argv[1], " requires a list", (char *) NULL);
      return TCL_ERROR;
    }

    if(mpsa_GetList(interp, argv[2], &List) != MPSA_OKAY) {
      return TCL_ERROR;
    }

    for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
      if(quadtree_DirectGravCalc(Link->Pcl, List) != QUADTREE_OKAY) {
	Tcl_AppendResult(interp, "Error calculating gravity", (char *) NULL);
	return TCL_ERROR;
      }
    }
  } else if((strcmp(argv[1], "Switch") == 0) ||
	    (strcmp(argv[1], "switch") == 0)) {
    if(argc != 3) {
      Tcl_AppendResult(interp, argv[1], " requires an option to set",
        (char *) NULL);
      return TCL_ERROR;
    }

    if(quadtree_SetOption(argv[2]) != QUADTREE_OKAY) {
      Tcl_AppendResult(interp, "Error setting criterion", (char *) NULL);
      return TCL_ERROR;
    }

    return TCL_OK;

  } else {
    Tcl_AppendResult(interp, "Option not recognised", (char *) NULL);
    return TCL_ERROR;
  }
}
