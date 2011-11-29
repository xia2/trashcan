/* pair lists for MPSA v1.0
 * maintained by g.winter
 * 15th january 2001
 * 
 * 
 */

#ifndef _PAIRLIST
#define _PAIRLIST

#define PAIRLIST_OKAY 0
#define PAIRLIST_FAIL 1

#include "mpsa_export.h"
#include "tcl.h"

typedef struct pairlist_List{
  struct pairlist_Link *firstLink;
  struct pairlist_Link *lastLink;
  int NElements;
} pairlist_List;

typedef struct pairlist_Link{
  struct pairlist_Link *nextLink;
  struct mpsa_Particle *PclA;
  struct mpsa_Particle *PclB;
  float index;
} pairlist_Link;

extern Tcl_HashTable pairlist_HashTable;

extern int pairlist_Create(
  Tcl_Interp *interp,
  char *name
);

extern int pairlist_Delete(
  Tcl_Interp *interp,
  char *name
);

extern int pairlist_GetPairlist(
  Tcl_Interp *interp,
  char *name,
  pairlist_List **List
);

extern int pairlist_Clear(
  pairlist_List *List
);

extern int pairlist_AppendLink(
  pairlist_List *List,
  mpsa_Particle *PclA,
  mpsa_Particle *PclB,
  float index
);

extern int pairlist_Order(
  pairlist_List *List
);

extern int pairlist_Sort(
  int NElements,
  float *Index,
  mpsa_Particle **PclArA,
  mpsa_Particle **PclArB
);

extern int pairlist_RemoveDoubles(
  pairlist_List *List
);

#endif
