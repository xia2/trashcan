/* celllist implementation for MPSA
 * 25th january 2001
 * maintained by g.winter
 * 
 */

#ifndef _CELLLIST
#define _CELLLIST

#define CELLLIST_OKAY 0
#define CELLLIST_FAIL 1

#include "mpsa_export.h"
#include "tcl.h"

extern Tcl_HashTable celllist_HashTable;

typedef struct celllist_Element{
  mpsa_Particle **List;
  int NinCell;
} celllist_Element;

typedef struct celllist_List{
  int Size[3];
  float GridSize[3];
  float Origin[3];
  celllist_Element ***Grid;
} celllist_List;

extern int celllist_celllistCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
);

extern int celllist_CreateCelllist(
  Tcl_Interp *interp,
  char *name
);

extern int celllist_DeleteCelllist(
  Tcl_Interp *interp,
  char *name
);

extern int celllist_ClearCelllist(
  celllist_List *List
);

extern int celllist_GetCelllist(
  Tcl_Interp *interp,
  char *Name,
  celllist_List **List
);

extern int celllist_SetGridSize(
  celllist_List *Celllist,
  mpsa_List *List,
  float dt
);

extern int celllist_LoadList(
  celllist_List *Celllist,
  mpsa_List *List
);

extern int celllist_AppendParticle(
  celllist_Element *Element,
  mpsa_Particle *Particle
);

extern int celllist_ClearElement(
  celllist_Element *Element
);

#endif
