/* pair lists for MPSA v1.0
 * maintained by g.winter
 * 15th january 2001
 * 
 * 
 */

#include "mpsa_export.h"
#include "tcl.h"
#include "pairlist_export.h"

/* routine to create a new pair list structure 
 * and register it in a hash table
 */

int pairlist_Create(
  Tcl_Interp *interp,
  char *name
)
{
  Tcl_HashEntry *Entry;
  int new;
  pairlist_List *List;

  List = (pairlist_List *) malloc (sizeof(pairlist_List));
  List->NElements = 0;
  List->firstLink = NULL;
  List->lastLink = NULL;

  Entry = Tcl_CreateHashEntry(&pairlist_HashTable, name, &new);
  if(new) {
    Tcl_SetHashValue(Entry, List);
  } else {
    free(List);
    Tcl_AppendResult(interp, name, " already used in hash table", 
      (char *) NULL);
    return PAIRLIST_FAIL;
  }

  return PAIRLIST_OKAY;
}

/* likewise, a routing to delete a pairlist structure */

int pairlist_Delete(
  Tcl_Interp *interp,
  char *name
)
{
  pairlist_List *List;
  Tcl_HashEntry *Entry;

  Entry = Tcl_FindHashEntry(&pairlist_HashTable, name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No pairlist called ", name, (char *) NULL);
    return PAIRLIST_FAIL;
  } else {
    List = (pairlist_List *) Tcl_GetHashValue(Entry);
  }

  pairlist_Clear(List);
  Tcl_DeleteHashEntry(Entry);
  free(List);

  return PAIRLIST_OKAY;
}

/* retrieve a pairlist from the hash table and return it in *List */

int pairlist_GetPairlist(
  Tcl_Interp *interp,
  char *name,
  pairlist_List **List
)
{
  Tcl_HashEntry *Entry;
  
  Entry = Tcl_FindHashEntry(&pairlist_HashTable, name);
  if(Entry == NULL) {
    Tcl_AppendResult(interp, "No pairlist called ", name, (char *) NULL);
    return PAIRLIST_FAIL;
  } else {
    *List = (pairlist_List *) Tcl_GetHashValue(Entry);
  }

  return PAIRLIST_OKAY;
}

/* empty all of the links from a pairlist */

int pairlist_Clear(
  pairlist_List *List
)
{
  pairlist_Link *ThisLink;
  pairlist_Link *NextLink;

  for(ThisLink = List->firstLink; ThisLink != NULL; ThisLink = NextLink) {
    NextLink = ThisLink->nextLink;
    free(ThisLink);
  }

  List->firstLink = NULL;
  List->lastLink = NULL;
  List->NElements = 0;

  return PAIRLIST_OKAY;
}

/* add a new link to the end of the pairlist */

int pairlist_AppendLink(
  pairlist_List *List,
  mpsa_Particle *PclA,
  mpsa_Particle *PclB,
  float index
)
{
  pairlist_Link *Link;

  Link = (pairlist_Link *) malloc (sizeof(pairlist_Link));
  Link->PclA = PclA;
  Link->PclB = PclB;
  Link->index = index;
  Link->nextLink = NULL;

  if(List->firstLink != NULL) {
    List->lastLink->nextLink = Link;
    List->lastLink = Link;
    List->NElements ++;
  } else {
    List->firstLink = Link;
    List->lastLink = Link;
    List->NElements ++;
  }

  return PAIRLIST_OKAY;
}

/* put the links in a list in order of their index */

int pairlist_Order(
  pairlist_List *List
)
{
  mpsa_Particle **PclArrayA;
  mpsa_Particle **PclArrayB;
  float *IndexArray;
  int i,n;

  pairlist_Link *Link;

  n = List->NElements + 1;

  IndexArray = (float *) malloc (sizeof(float) * n);
  PclArrayA = (mpsa_Particle **) malloc (sizeof(mpsa_Particle *) * n);
  PclArrayB = (mpsa_Particle **) malloc (sizeof(mpsa_Particle *) * n);

  /* copy linked pairlist into array */

  i = 1;
  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    IndexArray[i] = Link->index;
    PclArrayA[i] = Link->PclA;
    PclArrayB[i] = Link->PclB;
    i++;
  }

  pairlist_Sort(n - 1, IndexArray, PclArrayA, PclArrayB);

  /* copy array into linked pairlist */

  i = 1;
  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    Link->index = IndexArray[i];
    /* little modification 24th january to put the pointers in order */
    if((int) PclArrayA[i] < (int) PclArrayB[i]) {
      Link->PclA = PclArrayA[i];
      Link->PclB = PclArrayB[i];
    } else {
      Link->PclB = PclArrayA[i];
      Link->PclA = PclArrayB[i];
    }
    i++;
  }
  
  free(IndexArray);
  free(PclArrayA);
  free(PclArrayB);

  return PAIRLIST_OKAY;
}

/* the above routine uses this */

int pairlist_Sort(
  int n,
  float *Index,
  mpsa_Particle **PclArA,
  mpsa_Particle **PclArB
)
{
  /* taken nearly virbatim from Press et Al. */

  int l, j, ir, i;
  float r;
  mpsa_Particle *TempA, *TempB;

  l = (n >> 1) + 1;
  ir = n;
  for(;;) {
    if(l > 1) {
      r = Index[--l];
      TempA = PclArA[l];
      TempB = PclArB[l];
    } else {
      r = Index[ir];
      TempA = PclArA[ir];
      TempB = PclArB[ir];
      Index[ir] = Index[1];
      PclArA[ir] = PclArA[1];
      PclArB[ir] = PclArB[1];
      if(--ir == 1) {
	Index[1] = r;
	PclArA[1] = TempA;
	PclArB[1] = TempB;
	return PAIRLIST_OKAY;
      }
    }
    i = l;
    j = l << 1;
    while(j <= ir) {
      if(j < ir && Index[j] < Index[j+1]) {
	++j;
      }
      if(r < Index[j]) {
	Index[i] = Index[j];
	PclArA[i] = PclArA[j];
	PclArB[i] = PclArB[j];
	j += (i = j);
      } else {
	j = ir + 1;
      }
    }

    Index[i] = r;
    PclArA[i] = TempA;
    PclArB[i] = TempB;
  }

  return PAIRLIST_OKAY;
}

int pairlist_RemoveDoubles(
  pairlist_List *List
)
{
  pairlist_Link *ThisLink, *NextLink;
  int nremoved = 0;

  for(ThisLink = List->firstLink; ThisLink != NULL; ThisLink = 
    ThisLink->nextLink) {
    NextLink = ThisLink->nextLink;
    if(NextLink != NULL) {
      if((ThisLink->index == NextLink->index) &&
	 (ThisLink->PclA == NextLink->PclA) &&
	 (ThisLink->PclB == NextLink->PclB)) {
	/* remove nextlink */
	ThisLink->nextLink = NextLink->nextLink;
	nremoved ++;
	free(NextLink);
      }
    }
  }

  List->NElements -= nremoved;

  return PAIRLIST_OKAY;
}
