#include "tpp_export.h"
#include "mpsa_export.h"

/*[ tpp_PclInNode
 *[ action:  calculates whether a particle is within a node
 *[ objects: takes a particle and a node
 */

int tpp_PclInNode(
  mpsa_Particle *Pcl,
  tpp_Node *Node
)
{
  int i;
  float HalfSize;
  float HalfDV;
  HalfSize = Node->size_x * 0.5;
  HalfDV = Node->size_v * 0.5;
  for(i = 0; i < 3; i++) {
    if(fabs(Pcl->x[i] - Node->centre_x[i]) > HalfSize) {
      return TPP_FAIL;
    }
    if(fabs(Pcl->v[i] - Node->centre_v[i]) > HalfDV) {
      return TPP_FAIL;
    }
  }
  return TPP_OKAY;
}

/*[ tpp_WhichNode
 *[ action:  returns the number of the sub node which Pcl is in
 *[ objects: takes a node and a particle
 */

int tpp_WhichNode(
  mpsa_Particle *Pcl,
  tpp_Node *Node
)
{
  int ThisNode = 0;
  if(Pcl->x[0] > Node->centre_x[0]) {
    ThisNode += 1;
  }
  if(Pcl->x[1] > Node->centre_x[1]) {
    ThisNode += 2;
  }
  if(Pcl->x[2] > Node->centre_x[2]) {
    ThisNode += 4;
  }
  if(Pcl->v[0] > Node->centre_v[0]) {
    ThisNode += 8;
  }
  if(Pcl->v[1] > Node->centre_v[1]) {
    ThisNode += 16;
  }
  if(Pcl->v[2] > Node->centre_v[2]) {
    ThisNode += 32;
  }
  return ThisNode;
}

/*[ tpp_OpenNode
 *[ action:  create sixty four sub nodes to the node being opened and
 *[          move the leaf to the appropriate one
 *[ objects: takes a node
 */

int tpp_OpenNode(
  tpp_Node *Node
)
{
  int i, j, x, y, z;
  int vx, vy, vz;
  float HalfSize;
  float HalfDV;

  if(Node->Branch != NULL) {
    return TPP_FAIL;
  }

  HalfSize = 0.5 * Node->size_x;
  HalfDV = 0.5 * Node->size_v;

  Node->Branch = (tpp_Node *) malloc (sizeof(tpp_Node) * 64);
  for(i = 0; i < 64; i++) {
    Node->Branch[i].Branch = NULL;
    Node->Branch[i].Leaf = NULL;
    for(j = 0; j < 3; j++) {
      Node->Branch[i].centre_x[j] = Node->centre_x[j];
      Node->Branch[i].centre_v[j] = Node->centre_v[j];
    }
    Node->Branch[i].size_x = HalfSize;
    Node->Branch[i].size_v = HalfDV;
  }

  for(x = 0; x < 2; x++) {
    for(y = 0; y < 2; y++) {
      for(z = 0; z < 2; z++) {
	for(vx = 0; vx < 2; vx++) {
	  for(vy = 0; vy < 2; vy++) {
	    for(vz = 0; vz < 2; vz++) {
	      i = 32 * vz + 16 * vy + 8 * vx + 4 * z + 2 * y + x;
	      Node->Branch[i].centre_x[0] += (x - 0.5) * HalfSize;
	      Node->Branch[i].centre_x[1] += (y - 0.5) * HalfSize;
	      Node->Branch[i].centre_x[2] += (z - 0.5) * HalfSize;
	      Node->Branch[i].centre_v[0] += (vx - 0.5) * HalfDV;
	      Node->Branch[i].centre_v[1] += (vy - 0.5) * HalfDV;
	      Node->Branch[i].centre_v[2] += (vz - 0.5) * HalfDV;
	    }
	  }
	}
      }
    }
  }

  j = tpp_WhichNode(Node->Leaf, Node);
  Node->Branch[j].Leaf = Node->Leaf;
  Node->Leaf = NULL;
  return TPP_OKAY;
}

/*[ tpp_CloseNode
 *[ action:  reverse of open node!
 *[ objects: takes a node
 */

int tpp_CloseNode(
  tpp_Node *Node
)
{
  int i;
  Node->Leaf = NULL;
  if(Node->Branch == NULL) {
    return TPP_OKAY;
  } else {
    for(i = 0; i < 64; i++) {
      tpp_CloseNode(&(Node->Branch[i]));
    }
    free(Node->Branch);
    Node->Branch = NULL;
  } 
  return TPP_OKAY;
}

/*[ tpp_IsNodeOpen
 *[ action:  does exactly what it ways on the tin
 *[ objects: takes a node
 */

int tpp_IsNodeOpen(
  tpp_Node *Node
)
{
  if(Node->Branch == NULL) {
    return TPP_FAIL;
  } else {
    return TPP_OKAY;
  }
}

/*[ tpp_SetNodeSize
 *[ action:  set a tpp node so that it is big enough to contain a whole
 *[          list of particles
 *[ objects: takes a tpp node and a list of particles
 */

int tpp_SetNodeSize(
  tpp_Node *Node,
  mpsa_List *List
)
{
  mpsa_Link *Link;
  int i;
  float upper[3], lower[3], x, size_x = 0;
  float vupper[3], vlower[3], v, size_v = 0;

  /* change 29th september see if position of root node is important */
  /* if it is, will have to formulate a proper way of setting the    */
  /* position.                                                       */
  /* changed to barnes method - commented out the old for the moment */
  /* no put back with an option! */
 
  for(i = 0; i < 3; i++) {
    lower[i] = List->firstLink->Pcl->x[i];
    upper[i] = List->firstLink->Pcl->x[i];
    vlower[i] = List->firstLink->Pcl->v[i];
    vupper[i] = List->firstLink->Pcl->v[i];
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
      v = Link->Pcl->v[i];
      if(v < vlower[i]) {
	vlower[i] = v;
      }
      if(v > vupper[i]) {
	vupper[i] = v;
      }
    }
  }
    
  for(i = 0; i < 3; i++) {
    Node->centre_x[i] = 0.5 * (upper[i] + lower[i]);
    if((x = (upper[i] - lower[i])) > size_x) {
      size_x = x;
    }
    Node->centre_v[i] = 0.5 * (vupper[i] + vlower[i]);
    if((v = (vupper[i] - vlower[i])) > size_v) {
      size_v = v;
    }
  }
    
  Node->size_x = size_x;
  Node->size_v = size_v;
    
  return TPP_OKAY;
}

/*[ tpp_LoadList
 *[ action:  load a list of particles into a tpp
 *[ objects: takes a node and a list
 */

int tpp_LoadList(
  tpp_Node *Node,
  mpsa_List *List
)
{
  mpsa_Link *Link;
  mpsa_Particle *Pcl;
  tpp_Node *ThisNode;
  int i;

  tpp_SetNodeSize(Node, List);

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    ThisNode = Node;
    Pcl = Link->Pcl;

    while(tpp_IsNodeOpen(ThisNode) == TPP_OKAY) {
      i = tpp_WhichNode(Pcl, ThisNode);
      ThisNode = &(ThisNode->Branch[i]);
    }

    while(ThisNode->Leaf != NULL) {
      if(tpp_OpenNode(ThisNode) != TPP_OKAY) {
	return TPP_FAIL;
      }

      i = tpp_WhichNode(Pcl, ThisNode);
      ThisNode = &(ThisNode->Branch[i]);
    }

    ThisNode->Leaf = Pcl;
  }

  return TPP_OKAY;
}

int tpp_PrintTPP(
  tpp_Node *Node,
  Tcl_Interp *interp
)
{
  char position[40], velocity[40];
  char size_x[12], size_v[12];
  int i;

  sprintf(position, "%f %f %f ", Node->centre_x[0], Node->centre_x[1],
    Node->centre_x[2]);
  sprintf(velocity, "%f %f %f ", Node->centre_v[0], Node->centre_v[1],
    Node->centre_v[2]);
  sprintf(size_x, "%f ", Node->size_x);
  sprintf(size_v, "%f ", Node->size_v);

  Tcl_AppendResult(interp, position, size_x, velocity, size_v, (char *) NULL);
  if(Node->Branch != NULL) {
    for(i = 0; i < 64; i++) {
      tpp_PrintTPP(&(Node->Branch[i]), interp);
    }
  }

  return TPP_OKAY;
}
