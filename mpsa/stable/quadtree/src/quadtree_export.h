/* quadtree module header file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 */

#ifndef _QUADTREE
#define _QUADTREE

#define QUADTREE_OKAY 0
#define QUADTREE_FAIL 1

#include "tcl.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "mpsa_export.h"

typedef struct quadtree_Node{
  struct quadtree_Node *Trunk;
  struct quadtree_Node *Branch;
  struct mpsa_Particle *Leaf;
  float size;
  float centre[3];
  float mass;
  float com[3];
  float offset;
  float Qxx, Qyy, Qzz;
  float Qxy, Qxz, Qyz;
} quadtree_Node;

/*[ quadtree_CalcGrav
 *[ action:  calculate the force of gravity from a quadtree to one particle
 *[ objects: takes a particle and a quadtree
 */

int quadtree_CalcGrav(
  mpsa_Particle *Particle,
  quadtree_Node *Node
);

/*[ quadtree_DirectGravCalc
 *[ action:  calculate force directly using n^2 formulation
 *[ objects: takes a particle and a list of particles
 */

int quadtree_DirectGravCalc(
  mpsa_Particle *Pcl,
  mpsa_List *List
);

#endif
