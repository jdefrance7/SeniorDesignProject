#ifndef LIBCANARD_VERSION
#define LIBCANARD_VERSION 0

//------------------------------------------------------------------------------

/*
  Arduino style library to access Canard v0 (Legacy).

  All files pulled from submodule.
*/

//------------------------------------------------------------------------------

#include "canard.h"
#include "canard_internals.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

void printCanardFrame(CanardCANFrame* frame);

void printCanardInstance(CanardInstance* ins);

void printCanardRxTransfer(CanardRxTransfer* rxt);

void printCanardPoolAllocatorStatistics(CanardPoolAllocatorStatistics* stats);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // LIBCANARD_VERSION