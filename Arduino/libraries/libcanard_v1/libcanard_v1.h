#ifndef LIBCANARD_VERSION
#define LIBCANARD_VERSION 1

//------------------------------------------------------------------------------

/*
  Arduino style library to access Canard v1.

  All files pulled from submodule.
*/

//------------------------------------------------------------------------------

#include "canard.h"
#include "canard_dsdl.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

void printCanardFrame(CanardFrame* frame);

void printCanardTransfer(CanardTransfer* transfer);

void printCanardInstance(CanardInstance* ins);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // LIBCANARD_VERSION
