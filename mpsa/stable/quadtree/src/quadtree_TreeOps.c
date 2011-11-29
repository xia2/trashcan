/* quadtree module operations file v1.0
 * for loading into mpsa
 * maintained by g.winter
 * 1st september 2000
 * 
 * modified 25th september:
 * made tree-setsize randomized to prevent symmetries building up and
 * affecting the physics. note that the same change must be applied to 
 * quadtree, and this will prevent them from being identical in their operation
 * 
 * modified 29th september:
 * made tree setsize operate on the same premise as barnes did, removing
 * any randomness, should add a switch
 *
 * modified 2nd octoober:
 * modified tree to detect incest as a back up plan. this shouldn't happen
 * but just in case it does, have added this precaution
 *
 * additional modification checking that Node->Leaf != NULL before preceeding
 * with expensive calculations, later removed because they're probably
 * a waste of time.
 *
 * 5th october further modifications, see tree_TreeOps.c for more details
 */

#include "quadtree_private.h"
#include "mpsa_export.h"
#include <stdlib.h>

float QuadGravTheta = 0.5;
float QuadGravEpsilonSQ = 0.01;
int QuadtreeOpening = QUADTREEOPENINGOFFSET;
int QuadRootType = QUADTREEROOTEXACT;
int QuadtreeDist = QUADTREEOPENCOM;
float QuadBarnesSize = 1;

/*[ quadtree_SetGravParam
 *[ action:  set global gravity variables
 *[ objects: takes two floating variables
 */

int quadtree_SetGravParam(
  float theta,
  float eps
)
{
  QuadGravTheta = theta;
  QuadGravEpsilonSQ = eps * eps;
  return QUADTREE_OKAY;
}

/*[ quadtree_PclInNode
 *[ action:  calculates whether a particle is within a node
 *[ objects: takes a particle and a node
 */

int quadtree_PclInNode(
  mpsa_Particle *Pcl,
  quadtree_Node *Node
)
{
  int i;
  float HalfSize;

  HalfSize = Node->size * 0.5;
  for(i = 0; i < 3; i++) {
    if(fabs(Pcl->x[i] - Node->centre[i]) > HalfSize) {
      return QUADTREE_FAIL;
    }
  }
  return QUADTREE_OKAY;
}

/*[ quadtree_WhichNode
 *[ action:  returns the number of the sub node which Pcl is in
 *[ objects: takes a node and a particle
 */

int quadtree_WhichNode(
  mpsa_Particle *Pcl,
  quadtree_Node *Node
)
{
  int ThisNode = 0;
  if(Pcl->x[0] > Node->centre[0]) {
    ThisNode += 1;
  }
  if(Pcl->x[1] > Node->centre[1]) {
    ThisNode += 2;
  }
  if(Pcl->x[2] > Node->centre[2]) {
    ThisNode += 4;
  }
  return ThisNode;
}

/*[ quadtree_OpenNode
 *[ action:  create eight sub nodes to the node being opened and
 *[          move the leaf to the appropriate one
 *[ objects: takes a node
 */

int quadtree_OpenNode(
  quadtree_Node *Node
)
{
  int i, j, x, y, z;
  float HalfSize;

  if(Node->Branch != NULL) {
    return QUADTREE_FAIL;
  }

  HalfSize = 0.5 * Node->size;

  Node->Branch = (quadtree_Node *) malloc (sizeof(quadtree_Node) * 8);
  for(i = 0; i < 8; i++) {
    Node->Branch[i].Branch = NULL;
    Node->Branch[i].Leaf = NULL;
    for(j = 0; j < 3; j++) {
      Node->Branch[i].centre[j] = Node->centre[j];
      Node->Branch[i].com[j] = 0;
    }
    Node->Branch[i].size = HalfSize;
    Node->Branch[i].Trunk = Node;
    Node->Branch[i].mass = 0;
    Node->Branch[i].offset = 0;
    Node->Branch[i].Qxx = 0;
    Node->Branch[i].Qyy = 0;
    Node->Branch[i].Qzz = 0;
    Node->Branch[i].Qxy = 0;
    Node->Branch[i].Qxz = 0;
    Node->Branch[i].Qyz = 0;
  }

  for(x = 0; x < 2; x++) {
    for(y = 0; y < 2; y++) {
      for(z = 0; z < 2; z++) {
	i = 4 * z + 2 * y + x;
	Node->Branch[i].centre[0] += (x - 0.5) * HalfSize;
	Node->Branch[i].centre[1] += (y - 0.5) * HalfSize;
	Node->Branch[i].centre[2] += (z - 0.5) * HalfSize;
      }
    }
  }

  j = quadtree_WhichNode(Node->Leaf, Node);
  Node->Branch[j].Leaf = Node->Leaf;
  Node->Leaf = NULL;
  return QUADTREE_OKAY;
}

/*[ quadtree_CloseNode
 *[ action:  reverse of open node!
 *[ objects: takes a node
 */

int quadtree_CloseNode(
  quadtree_Node *Node
)
{
  int i;
  Node->Leaf = NULL;
  if(Node->Branch == NULL) {
    return QUADTREE_OKAY;
  } else {
    for(i = 0; i < 8; i++) {
      quadtree_CloseNode(&(Node->Branch[i]));
    }
    free(Node->Branch);
    Node->Branch = NULL;
  } 
  return QUADTREE_OKAY;
}

/*[ quadtree_IsNodeOpen
 *[ action:  does exactly what it ways on the tin
 *[ objects: takes a node
 */

int quadtree_IsNodeOpen(
  quadtree_Node *Node
)
{
  if(Node->Branch == NULL) {
    return QUADTREE_FAIL;
  } else {
    return QUADTREE_OKAY;
  }
}

/*[ quadtree_SetNodeSize
 *[ action:  set a quadtree node so that it is big enough to contain a whole
 *[          list of particles
 *[ objects: takes a quadtree node and a list of particles
 */

int quadtree_SetNodeSize(
  quadtree_Node *Node,
  mpsa_List *List
)
{
  mpsa_Link *Link;
  int i;
  float upper[3], lower[3], x, size = 0;

  if(QuadRootType == QUADTREEROOTEXACT) {
    
    for(i = 0; i < 3; i++) {
      upper[i] = List->firstLink->Pcl->x[i];
      lower[i] = List->firstLink->Pcl->x[i];
    }
    
    for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
      for(i = 0; i < 3; i++) {
	x = Link->Pcl->x[i];
	if(x < lower[i]) {
	  lower[i] = x;
	}
	if(x > upper[i]) {
	  upper[i] = x;
	}
      }
    }
    
    for(i = 0; i < 3; i++) {
      Node->centre[i] = 0.5 * (upper[i] + lower[i]);
      if((x = upper[i] - lower[i]) > size) {
	size = x;
      }
    }
    
    Node->size = size;

    return QUADTREE_OKAY;

  } else {

    size = QuadBarnesSize;
    
    for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
      for(i = 0; i < 3; i++) {
	if(fabs(Link->Pcl->x[i]) > size) {
	  size *= 2;
	}
      }
    }
    
    for(i = 0; i < 3; i++) {
      Node->centre[i] = 0;
    }  
    
    Node->size = 2 * size;
    
    QuadBarnesSize = size;

    return QUADTREE_OKAY;
  }
}

/*[ quadtree_LoadList
 *[ action:  load a list of particles into a quadtree
 *[ objects: takes a node and a list
 */

int quadtree_LoadList(
  quadtree_Node *Node,
  mpsa_List *List
)
{
  mpsa_Link *Link;
  mpsa_Particle *Pcl;
  quadtree_Node *ThisNode;
  int i;

  quadtree_SetNodeSize(Node, List);

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    ThisNode = Node;
    Pcl = Link->Pcl;

    while(quadtree_IsNodeOpen(ThisNode) == QUADTREE_OKAY) {
      i = quadtree_WhichNode(Pcl, ThisNode);
      ThisNode = &(ThisNode->Branch[i]);
    }

    while(ThisNode->Leaf != NULL) {
      if(quadtree_OpenNode(ThisNode) != QUADTREE_OKAY) {
	return QUADTREE_FAIL;
      }

      i = quadtree_WhichNode(Pcl, ThisNode);
      ThisNode = &(ThisNode->Branch[i]);
    }

    ThisNode->Leaf = Pcl;
  }

  return QUADTREE_OKAY;

}

/*[ quadtree_CalcCOM
 *[ action:  calculate the centres of mass of a quadtree
 *[ objects: takes a quadtree node
 */

int quadtree_CalcCOM(
  quadtree_Node *Node
)
{
  float mass = 0, m;
  float drmax = 0;
  float x[3], drtemp;
  int i, j;
  float dx, dy, dz, drsq;
  float Qxx = 0, Qyy = 0, Qzz = 0, Qxy = 0, Qxz = 0, Qyz = 0;

  for(i = 0; i < 3; i++) {
    x[i] = 0;
  }

  if(quadtree_IsNodeOpen(Node) == QUADTREE_OKAY) {
    for(i = 0; i < 8; i++) {
      if(quadtree_CalcCOM(&(Node->Branch[i])) != QUADTREE_OKAY) {
	return QUADTREE_FAIL;
      }
      mass += Node->Branch[i].mass;
      for(j = 0; j < 3; j++) {
	x[j] += Node->Branch[i].mass * Node->Branch[i].com[j];
      }
      Qxx += Node->Branch[i].Qxx;
      Qyy += Node->Branch[i].Qyy;
      Qzz += Node->Branch[i].Qzz;
      Qxy += Node->Branch[i].Qxy;
      Qxz += Node->Branch[i].Qxz;
      Qyz += Node->Branch[i].Qyz;
    }
    for(j = 0; j < 3; j++) {
      Node->com[j] = x[j] / mass;
    }
    for(i = 0; i < 8; i++) {
      m = Node->Branch[i].mass;
      dx = Node->Branch[i].com[0] - Node->com[0];
      dy = Node->Branch[i].com[1] - Node->com[1];
      dz = Node->Branch[i].com[2] - Node->com[2];
      drsq = dx * dx + dy * dy + dz * dz;

      Qxx += m * (3 * dx * dx - drsq);
      Qyy += m * (3 * dy * dy - drsq);
      Qzz += m * (3 * dz * dz - drsq);
      Qxy += m * 3 * dx * dy;
      Qxz += m * 3 * dx * dz;
      Qyz += m * 3 * dy * dz;
    }
    Node->mass = mass;
    Node->Qxx = Qxx;
    Node->Qyy = Qyy;
    Node->Qzz = Qzz;
    Node->Qxy = Qxy;
    Node->Qxz = Qxz;
    Node->Qyz = Qyz;
    drmax = 0;
    for(j = 0; j < 3; j++) {
      drtemp = Node->com[j] - Node->centre[j] + 0.5 * Node->size;
      if(drtemp > (Node->size - drtemp)) {
	drmax += drtemp * drtemp;
      } else {
	drmax += (Node->size - drtemp) * (Node->size - drtemp);
      }
    }
    Node->offset = sqrt(drmax);
  } else if(Node->Leaf != NULL) {
    Node->mass = Node->Leaf->mass;
    for(j = 0; j < 3; j++) {
      Node->com[j] = Node->Leaf->x[j];
    }
  } else {
    Node->mass = 0;
    for(j = 0; j < 3; j++) {
      Node->com[j] = 0;
    }
  }
  return QUADTREE_OKAY;
}

/*[ quadtree_CalcGrav
 *[ action:  calculate the force of gravity from a quadtree to one particle
 *[ objects: takes a particle and a quadtree
 */

int quadtree_CalcGrav(
  mpsa_Particle *Particle,
  quadtree_Node *Node
)
{
  int i;
  float dsq, dr, dx[3], compare, dsqtwo;
  float force, PNDistSQ;
  float rQr, Qr[3], drfifth;

  if(Node->mass == 0) {
    return QUADTREE_OKAY;
  }

  if(Node->Leaf == Particle) {
    return QUADTREE_OKAY;
  }
  dsq = 0;
  for(i = 0; i < 3; i++) {
    dx[i] = (Particle->x[i] - Node->com[i]);
    dsq += dx[i] * dx[i];
  }

  if(QuadtreeOpening == QUADTREEOPENINGBASIC) {
    compare = Node->size;
  } else if(QuadtreeOpening == QUADTREEOPENINGOFFSET) {
    compare = Node->offset;
  } else {
    compare = Node->size;
  }

  if(QuadtreeDist == QUADTREEOPENCOM) {
    PNDistSQ = dsq;
  } else {
    PNDistSQ = 0;
    for(i = 0; i < 3; i++) {
      PNDistSQ += (Particle->x[i] - Node->centre[i]) * 
        (Particle->x[i] - Node->centre[i]);
    }
  }

  /* enable speeded up checks if theta < 1 */
  /* note - theta >= 1 for the foolhardy */

  if(QuadGravTheta >= 1) {
    if((QuadGravTheta * QuadGravTheta * PNDistSQ) < (compare * compare)) {
      
      /* node too close to leave closed so open it */
      
      if(quadtree_IsNodeOpen(Node) == QUADTREE_OKAY) {
	
	/* if it's openable, open it */
	
	for(i = 0; i < 8; i++) {
	quadtree_CalcGrav(Particle, &(Node->Branch[i]));
	}
      } else {
	
	/* can't open it as it is just a particle */
	/* do not want to calculate quadrupole moments for single particles */
	/* therefore no Q stuff                                             */
	
	/* if((Particle != Node->Leaf) && (Node->Leaf != NULL)) is redundant */
	/* if it is NULL, we don't care as it will have no effect on the     */
	/* result, and it can't be Pcl */
	
	dsqtwo = dsq + QuadGravEpsilonSQ;
	dr = sqrt(dsqtwo);
	force = Node->mass / dsqtwo;
	for(i = 0; i < 3; i++) {
	  Particle->a[i] -= force * dx[i] / dr;
	}
	
	Particle->phi -= Node->mass / dr;
      }
    } else if(quadtree_PclInNode(Particle, Node) != QUADTREE_OKAY) {
      
      /* if node is far enough away and particle is not in it */
      
      Qr[0] = Node->Qxx * dx[0] + Node->Qxy * dx[1] + Node->Qxz * dx[2];
      Qr[1] = Node->Qxy * dx[0] + Node->Qyy * dx[1] + Node->Qyz * dx[2];
      Qr[2] = Node->Qxz * dx[0] + Node->Qyz * dx[1] + Node->Qzz * dx[2];
      
      rQr = dx[0] * Qr[0] + dx[1] * Qr[1] + dx[2] * Qr[2];
      
      dsqtwo = dsq + QuadGravEpsilonSQ;
      dr = sqrt(dsqtwo);
      
      drfifth = dr * dsqtwo * dsqtwo;
      
      force = Node->mass / dsqtwo;
      for(i = 0; i < 3; i++) {
	Particle->a[i] += - (force * dx[i] / dr) + (Qr[i] / drfifth) - 
	  (2.5 * rQr * dx[i] / (drfifth * dsqtwo));
      }
      
      Particle->phi +=  - (Node->mass / dr) - (rQr / drfifth);
    } else {
      
      /* else if particle is in the node MUST open it to prevent tree incest */
      
      if(quadtree_IsNodeOpen(Node) == QUADTREE_OKAY) {
	for(i = 0; i < 8; i++) {
	  quadtree_CalcGrav(Particle, &(Node->Branch[i]));
	}
      } else {
	
	/* do not want to calculate quadrupole moments for single particles */
	/* therefore no Q stuff                                             */
	/* see above for discussion on checks */
	
	dsqtwo = dsq + QuadGravEpsilonSQ;
	dr = sqrt(dsqtwo);
	force = Node->mass / dsqtwo;
	for(i = 0; i < 3; i++) {
	  Particle->a[i] -= force * dx[i] / dr;
	}
	
	Particle->phi -= Node->mass / dr;
      }
    }
  } else {

    /* we are working with a sensible value of theta here */

    if((QuadGravTheta * QuadGravTheta * PNDistSQ) < (compare * compare)) {
      
      /* node too close to leave closed so open it */
      
      if(quadtree_IsNodeOpen(Node) == QUADTREE_OKAY) {
	
	/* if it's openable, open it */
	
	for(i = 0; i < 8; i++) {
	quadtree_CalcGrav(Particle, &(Node->Branch[i]));
	}
      } else {
	
	/* can't open it as it is just a particle */
	/* do not want to calculate quadrupole moments for single particles */
	/* therefore no Q stuff                                             */
	
	/* if((Particle != Node->Leaf) && (Node->Leaf != NULL)) is redundant */
	/* if it is NULL, we don't care as it will have no effect on the     */
	/* result, and it can't be Pcl */
	
	dsqtwo = dsq + QuadGravEpsilonSQ;
	dr = sqrt(dsqtwo);
	force = Node->mass / dsqtwo;
	for(i = 0; i < 3; i++) {
	  Particle->a[i] -= force * dx[i] / dr;
	}
	
	Particle->phi -= Node->mass / dr;
      }

    } else {
      
      /* if node is far enough away and particle is not in it */
      
      Qr[0] = Node->Qxx * dx[0] + Node->Qxy * dx[1] + Node->Qxz * dx[2];
      Qr[1] = Node->Qxy * dx[0] + Node->Qyy * dx[1] + Node->Qyz * dx[2];
      Qr[2] = Node->Qxz * dx[0] + Node->Qyz * dx[1] + Node->Qzz * dx[2];
      
      rQr = dx[0] * Qr[0] + dx[1] * Qr[1] + dx[2] * Qr[2];
      
      dsqtwo = dsq + QuadGravEpsilonSQ;
      dr = sqrt(dsqtwo);
      
      drfifth = dr * dsqtwo * dsqtwo;
      
      force = Node->mass / dsqtwo;
      for(i = 0; i < 3; i++) {
	Particle->a[i] += - (force * dx[i] / dr) + (Qr[i] / drfifth) - 
	  (2.5 * rQr * dx[i] / (drfifth * dsqtwo));
      }
      
      Particle->phi +=  - (Node->mass / dr) - (rQr / drfifth);
    } 
  }

  return QUADTREE_OKAY;
}

/*[ quadtree_DirectGravCalc
 *[ action:  calculate force directly using n^2 formulation
 *[ objects: takes a particle and a list of particles
 */

int quadtree_DirectGravCalc(
  mpsa_Particle *Pcl,
  mpsa_List *List
)
{
  int i;
  float dsq, dr, dx[3], force;
  mpsa_Link *Link;

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    if(Link->Pcl == Pcl) {
      /* do nothing */
    } else {
      dsq = 0;
      for(i = 0; i < 3; i++) {
	dx[i] = Pcl->x[i] - Link->Pcl->x[i];
	dsq += dx[i] * dx[i];
      }
      dsq += QuadGravEpsilonSQ;
      dr = sqrt(dsq);
      force = Link->Pcl->mass / dsq;
      for(i = 0; i < 3; i++) {
	Pcl->a[i] -= force * dx[i] / dr;
      }
    }
  }

  return QUADTREE_OKAY;
}

/*[ quadtree_SetOption
 *[ action:  set one of the many flags in the quadtree code
 *[ objects: takes a name of a flag to set
 */

extern int quadtree_SetOption(
  char *Option
)
{
  if((strcmp(Option, "OpenBasic") == 0) ||
     (strcmp(Option, "openbasic") == 0)) {
    QuadtreeOpening = QUADTREEOPENINGBASIC;
  } else if((strcmp(Option, "OpenSW") == 0) ||
	    (strcmp(Option, "opensw") == 0)) {
    QuadtreeOpening = QUADTREEOPENINGOFFSET;
  } else if((strcmp(Option, "DistGeom") == 0) ||
	    (strcmp(Option, "distgeom") == 0)) {
    QuadtreeDist = QUADTREEOPENGEOM;
  } else if((strcmp(Option, "DistCOM") == 0) ||
	    (strcmp(Option, "distcom") == 0)) {
    QuadtreeDist = QUADTREEOPENCOM;
  } else if((strcmp(Option, "RootBarnes") == 0) ||
	    (strcmp(Option, "rootbarnes") == 0)) {
    QuadRootType = QUADTREEROOTBARNES;
  } else if((strcmp(Option, "RootExact") == 0) ||
	    (strcmp(Option, "rootexact") == 0)) {
    QuadRootType = QUADTREEROOTEXACT;
  } else {
    return QUADTREE_FAIL;
  }

  return QUADTREE_OKAY;
}
