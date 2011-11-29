/* diagnostic module for MPSA
 * g.winter
 */

#include "mpsa_export.h"
#include <math.h>
#include "diagnostic_export.h"
#include "tcl.h"

int diagnostic_diagnosticCmd(
  ClientData dummy,
  Tcl_Interp *interp,
  int argc,
  char **argv
)
{
  mpsa_List *List;
  mpsa_Link *Link;
  float phi[30];	/* potential */
  float v_para[30];	/* velocity || to r */
  float m[30];		/* mass */
  float v_perp[30];	/* velocity -| to r */
  float r, rx, ry, vdotr;
  float kappa, omega, dodr, dsqodrsq;
  char result[80];
  int i;

  if(argc < 2) {
    Tcl_AppendResult(interp, argv[0], " requires a list", (char *) NULL);
    return TCL_ERROR;
  }

  if(mpsa_GetList(interp, argv[1], &List) != MPSA_OKAY) {
    return TCL_ERROR;
  }

  for(i = 0; i < 30; i++) {
    v_para[i] = 0;
    v_perp[i] = 0;
    phi[i] = 0;
    m[i] = 0;
  }

  for(Link = List->firstLink; Link != NULL; Link = Link->nextLink) {
    r = sqrt(Link->Pcl->x[0] * Link->Pcl->x[0] +
      Link->Pcl->x[1] * Link->Pcl->x[1]);
    rx = Link->Pcl->x[0] / r;
    ry = Link->Pcl->x[1] / r;

    vdotr = rx * Link->Pcl->v[0] + ry * Link->Pcl->v[1];

    i = r * 2;
    if(i < 30) {
      m[i] += Link->Pcl->mass;
      phi[i] += Link->Pcl->mass * Link->Pcl->phi;
      v_para[i] += Link->Pcl->mass * vdotr;
      v_perp[i] += Link->Pcl->mass * sqrt(Link->Pcl->v[0] * 
        Link->Pcl->v[0] + Link->Pcl->v[1] * Link->Pcl->v[1] - vdotr * vdotr);
    }
  }

  for(i = 1; i < 29; i++) {
    if(m[i] != 0) {
      v_para[i] /= m[i];
      v_perp[i] /= m[i];
      phi[i] /= m[i];
    }

    dodr = phi[i+1] - phi[i - 1];
    dsqodrsq = 4 * (phi[i + 1] - 2 * phi[i] + phi[i - 1]);
    kappa = sqrt(dsqodrsq + 3 * dodr / (0.5 * ((float) i + 0.5)));

    sprintf(result, "%e %e %e %e %e %e\n", 0.5 * ((float) i + 0.5), m[i], 
      dodr / (0.5 * ((float) i + 0.5)), v_perp[i] / (0.5 * ((float) i + 
      0.5)), phi[i], kappa);
    Tcl_AppendResult(interp, result, (char *) NULL);
  }

  return TCL_OK;
}
