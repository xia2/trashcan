#include "diagnostic_export.h"
#include <tcl.h>

int Diagnostic_Init(
  Tcl_Interp *interp
)
{
  Tcl_CreateCommand(interp, "::mpsa::diagnostic", diagnostic_diagnosticCmd,
    (ClientData) NULL, (void(*)()) NULL);

  return TCL_OK;
}