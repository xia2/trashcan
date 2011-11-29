/* tree ++ header file v0.1
 * invented by g.winter
 * 15th january 2001
 * 
 * 
 */

#ifndef _TPP
#define _TPP

/* tree ++
 * 6 dimensional tree structure for serious searching
 * particularly for finding collisions between particles
 * which have a well defined cross section. 
 * (c) 2001 g.winter
 */

#include "tcl.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "mpsa_export.h"

#define TPP_OKAY 0
#define TPP_FAIL 1

typedef struct tpp_Node{
  struct tpp_Node *Branch;
  struct mpsa_Particle *Leaf;
  float size_x;
  float size_v;
  float centre_x[3];
  float centre_v[3];
} tpp_Node;

extern Tcl_HashTable tpp_TppHashTable;

/*[ tpp_PclInNode
 *[ action:  calculates whether a particle is within a node
 *[ objects: takes a particle and a node
 */

extern int tpp_PclInNode(
  mpsa_Particle *Pcl,
  tpp_Node *Node
);

/*[ tpp_WhichNode
 *[ action:  returns the number of the sub node which Pcl is in
 *[ objects: takes a node and a particle
 */

extern int tpp_WhichNode(
  mpsa_Particle *Pcl,
  tpp_Node *Node
);

/*[ tpp_OpenNode
 *[ action:  create eight sub nodes to the node being opened and
 *[          move the leaf to the appropriate one
 *[ objects: takes a node
 */

extern int tpp_OpenNode(
  tpp_Node *Node
);

/*[ tpp_CloseNode
 *[ action:  reverse of open node!
 *[ objects: takes a node
 */

extern int tpp_CloseNode(
  tpp_Node *Node
);

/*[ tpp_IsNodeOpen
 *[ action:  does exactly what it ways on the tin
 *[ objects: takes a node
 */

extern int tpp_IsNodeOpen(
  tpp_Node *Node
);

/* Tcl level/hash table things */

/*[ tpp_CreateTpp
 *[ action:  creates a new 'blank' tpp trunk and pops it into a hash
 *[          table
 *[ objects: takes an interpreter and a name for the tpp
 */

extern int tpp_CreateTpp(
  Tcl_Interp *interp,
  char *Name
);

/*[ tpp_GetTpp
 *[ action:  retrieves a tpp pointer from a hash table
 *[ objects: takes an interpreter, name of a tpp and returns a pointer
 */

extern int tpp_GetTpp(
  Tcl_Interp *interp,
  char *Name,
  tpp_Node **Root
);

/*[ tpp_DeleteTpp
 *[ action:  removes the hash table entry and data from a tpp
 *[ objects: takes a name
 */

extern int tpp_DeleteTpp(
  Tcl_Interp *interp,
  char *Name
);

/*[ tpp_SetNodeSize
 *[ action:  set a tpp node so that it is big enough to contain a whole
 *[          list of particles
 *[ objects: takes a tpp node and a list of particles
 */

extern int tpp_SetNodeSize(
  tpp_Node *Node,
  mpsa_List *List
);

/*[ tpp_LoadList
 *[ action:  load a list of particles into a tpp
 *[ objects: takes a node and a list
 */

extern int tpp_LoadList(
  tpp_Node *Node,
  mpsa_List *List
);

extern int tpp_PrintTPP(
  tpp_Node *Node,
  Tcl_Interp *interp
);

/*[ tpp_TppCmd
 *[ action:  anything to do with tpps
 *[ objects: probably takes tpps and lists
 *[ syntax:  Tpp ...stuff...
 */

extern int tpp_TppCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
);

#endif
