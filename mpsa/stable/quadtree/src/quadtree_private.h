/* quadtree module internal header file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 */

#ifndef _QUADTREE_PRIV
#define _QUADTREE_PRIV

#define QUADTREEOPENINGBASIC 100
#define QUADTREEOPENINGOFFSET 101
#define QUADTREEOPENGEOM 110
#define QUADTREEOPENCOM 111
#define QUADTREEROOTEXACT 120
#define QUADTREEROOTBARNES 121

#include "quadtree_export.h"

extern Tcl_HashTable quadtree_QuadtreeHashTable;

/*[ quadtree_SetGravParam
 *[ action:  set global gravity variables
 *[ objects: takes two floating variables
 */

int quadtree_SetGravParam(
  float theta,
  float eps
);

/*[ quadtree_PclInNode
 *[ action:  calculates whether a particle is within a node
 *[ objects: takes a particle and a node
 */

extern int quadtree_PclInNode(
  mpsa_Particle *Pcl,
  quadtree_Node *Node
);

/*[ quadtree_WhichNode
 *[ action:  returns the number of the sub node which Pcl is in
 *[ objects: takes a node and a particle
 */

extern int quadtree_WhichNode(
  mpsa_Particle *Pcl,
  quadtree_Node *Node
);

/*[ quadtree_OpenNode
 *[ action:  create eight sub nodes to the node being opened and
 *[          move the leaf to the appropriate one
 *[ objects: takes a node
 */

extern int quadtree_OpenNode(
  quadtree_Node *Node
);

/*[ quadtree_CloseNode
 *[ action:  reverse of open node!
 *[ objects: takes a node
 */

extern int quadtree_CloseNode(
  quadtree_Node *Node
);

/*[ quadtree_IsNodeOpen
 *[ action:  does exactly what it ways on the tin
 *[ objects: takes a node
 */

extern int quadtree_IsNodeOpen(
  quadtree_Node *Node
);

/*[ quadtree_CreateQuadtree
 *[ action:  creates a new 'blank' quadtree trunk and pops it into a hash
 *[          table
 *[ objects: takes an interpreter and a name for the quadtree
 */

extern int quadtree_CreateQuadtree(
  Tcl_Interp *interp,
  char *Name
);

/*[ quadtree_GetQuadtree
 *[ action:  retrieves a quadtree pointer from a hash table
 *[ objects: takes an interpreter, name of a quadtree and returns a pointer
 */

extern int quadtree_GetQuadtree(
  Tcl_Interp *interp,
  char *Name,
  quadtree_Node **Root
);

/*[ quadtree_DeleteQuadtree
 *[ action:  removes the hash table entry and data from a quadtree
 *[ objects: takes a name
 */

extern int quadtree_DeleteQuadtree(
  Tcl_Interp *interp,
  char *Name
);

/*[ quadtree_SetNodeSize
 *[ action:  set a quadtree node so that it is big enough to contain a whole
 *[          list of particles
 *[ objects: takes a quadtree node and a list of particles
 */

extern int quadtree_SetNodeSize(
  quadtree_Node *Node,
  mpsa_List *List
);

/*[ quadtree_LoadList
 *[ action:  load a list of particles into a quadtree
 *[ objects: takes a node and a list
 */

extern int quadtree_LoadList(
  quadtree_Node *Node,
  mpsa_List *List
);

/*[ quadtree_QuadtreeCmd
 *[ action:  anything to do with quadtrees
 *[ objects: probably takes quadtrees and lists
 *[ syntax:  Quadtree ...stuff...
 */

extern int quadtree_QuadtreeCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
);

/*[ quadtree_CalcCOM
 *[ action:  calculate the centres of mass of a quadtree
 *[ objects: takes a quadtree node
 */

extern int quadtree_CalcCOM(
  quadtree_Node *Node
);

/*[ quadtree_SetOption
 *[ action:  set one of the many flags in the quadtree code
 *[ objects: takes a name of a flag to set
 */

extern int quadtree_SetOption(
  char *Option
);

#endif
