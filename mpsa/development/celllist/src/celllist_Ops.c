/* celllist implementation for MPSA
 * 25th january 2001
 * maintained by g.winter
 * 
 */

#include "mpsa_export.h"
#include "celllist_export.h"
#include "tcl.h"
#include <math.h>

int celllist_CreateCelllist(
  Tcl_Interp *interp,
  char *name
)
{
  Tcl_HashEntry *Entry;
  int new, i;
  celllist_List *List;
  
  List = (celllist_List *) malloc (sizeof(celllist_List));
  for(i = 0; i < 3; i++) {
    List->Size[i] = 0;
    List->Origin[i] = 0;
    List->GridSize[i] = 0;
  }

  List->Grid = NULL;

  Entry = Tcl_CreateHashEntry(&celllist_HashTable, name, &new);
  if(new) {
    Tcl_SetHashValue(Entry, List);
  } else {
    free(List);
    Tcl_AppendResult(interp, "Error creating hash table entry", (char *) NULL);
    return CELLLIST_FAIL;
  }

  return CELLLIST_OKAY;
}

int celllist_DeleteCelllist(
  Tcl_Interp *interp,
  char *name
)
{
  celllist_List *List;
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&celllist_HashTable, name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No celllist of that name in the hash table",
      (char *) NULL);
    return CELLLIST_FAIL;
  } else {
    List = (celllist_List *) Tcl_GetHashValue(Entry);
  }

  celllist_ClearCelllist(List);
  Tcl_DeleteHashEntry(Entry);
  free(List);

  return CELLLIST_OKAY;
}

int celllist_ClearCelllist(
  celllist_List *List
)
{
  int i, j, k;

  if(List->Grid == NULL) {
    return CELLLIST_OKAY;
  }

  for(i = 0; i < List->Size[0]; i++) {
    for(j = 0; j < List->Size[1]; j++) {
      for(k = 0; k < List->Size[2]; k++) {
	celllist_ClearElement(&(List->Grid[i][j][k]));
      }
      free(List->Grid[i][j]);
    }
    free(List->Grid[i]);
  }
  free(List->Grid);

  for(i = 0; i < 3; i++) {
    List->Size[i] = 0;
    List->Origin[i] = 0;
    List->GridSize[i] = 0;
  }

  List->Grid = NULL;

  return CELLLIST_OKAY;
}

int celllist_GetCelllist(
  Tcl_Interp *interp,
  char *name,
  celllist_List **List
)
{
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&celllist_HashTable, name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No celllist of that name in the hash table",
      (char *) NULL);
    return CELLLIST_FAIL;
  } else {
    *List = (celllist_List *) Tcl_GetHashValue(Entry);
  }

  return CELLLIST_OKAY;
}

int celllist_SetGridSize(
  celllist_List *Celllist,
  mpsa_List *List,
  float dt
)
{
  mpsa_Link *Link;
  float min[3], max[3], meanv[3], meanvsq[3], sdv[3];
  int nx, ny, nz, i, j, k;
  float spacing[3];
  int nparticles = 0;

  if(List->NElements == 0) {
    return CELLLIST_OKAY;
  }

  for(i = 0; i < 3; i++) {
    min[i] = List->firstLink->Pcl->x[i];
    max[i] = List->firstLink->Pcl->x[i];
    meanv[i] = 0;
    meanvsq[i] = 0;
    sdv[i] = 0;
  }

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    for(i = 0; i < 3; i++) {
      if(Link->Pcl->x[i] > max[i]) {
	max[i] = Link->Pcl->x[i];
      }
      if(Link->Pcl->x[i] < min[i]) {
	min[i] = Link->Pcl->x[i];
      }
      meanv[i] += Link->Pcl->v[i];
      meanvsq[i] += Link->Pcl->v[i] * Link->Pcl->v[i];
    }
    nparticles ++;
  }

  for(i = 0; i < 3; i++) {
    sdv[i] = sqrt((meanvsq[i] / nparticles) - (meanv[i] / nparticles) * 
      (meanv[i] / nparticles));
  }

  for(i = 0; i < 3; i++) {
    spacing[i] = 3 * sdv[i] * dt;
  }

  nx = 1 + (max[0] - min[0]) / spacing[0];
  ny = 1 + (max[1] - min[1]) / spacing[1];
  nz = 1 + (max[2] - min[2]) / spacing[2];

  Celllist->Grid = (celllist_Element ***) malloc 
    (sizeof(celllist_Element **) * nx);
  for(i = 0; i < nx; i++) {
    Celllist->Grid[i] = (celllist_Element **) malloc 
      (sizeof(celllist_Element *) * ny);
    for(j = 0; j < ny; j++) {
      Celllist->Grid[i][j] = (celllist_Element *) malloc 
	(sizeof(celllist_Element) * nz);
      for(k = 0; k < nz; k++) {
	Celllist->Grid[i][j][k].NinCell = 0;
	Celllist->Grid[i][j][k].List = NULL;
      }
    }
  }

  Celllist->Size[0] = nx;
  Celllist->Size[1] = ny;
  Celllist->Size[2] = nz;

  for(i = 0; i < 3; i++) {
    Celllist->GridSize[i] = spacing[i];
    Celllist->Origin[i] = min[i];
  }

  return CELLLIST_OKAY;
}
  
int celllist_LoadList(
  celllist_List *Celllist,
  mpsa_List *List
)
{
  int i, j, k;
  float origin[3], gridsize[3];
  mpsa_Link *Link;

  if(Celllist->Grid == NULL) {
    return CELLLIST_FAIL;
  }

  for(i = 0; i < 3; i++) {
    origin[i] = Celllist->Origin[i];
    gridsize[i] = Celllist->GridSize[i];
  }

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    i = (Link->Pcl->x[0] - origin[0]) / gridsize[0];
    j = (Link->Pcl->x[1] - origin[1]) / gridsize[1];
    k = (Link->Pcl->x[2] - origin[2]) / gridsize[2];
    celllist_AppendParticle(&(Celllist->Grid[i][j][k]), Link->Pcl);
  }

  return CELLLIST_OKAY;
}
  
int celllist_AppendParticle(
  celllist_Element *Element,
  mpsa_Particle *Particle
)
{
  int ncurrent, i;
  mpsa_Particle **temp;

  if(Element->List == NULL) {
    Element->List = (mpsa_Particle **) malloc (sizeof(mpsa_Particle *));
    Element->NinCell = 1;
    Element->List[0] = Particle;
  } else {
    ncurrent = Element->NinCell;
    temp = (mpsa_Particle **) malloc (sizeof(mpsa_Particle *) * 
      (ncurrent + 1));
    for(i = 0; i < ncurrent; i++) {
      temp[i] = Element->List[i];
    }
    temp[ncurrent] = Particle;
    free(Element->List);
    Element->List = temp;
    Element->NinCell = ncurrent + 1;
  }

  return CELLLIST_OKAY;
}

int celllist_ClearElement(
  celllist_Element *Element
)
{
  if(Element->List != NULL) {
    free(Element->List);
    Element->List = NULL;
  }

  Element->NinCell = 0;
  
  return CELLLIST_OKAY;
}
