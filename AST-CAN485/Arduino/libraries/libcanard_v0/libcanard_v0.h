/**
 * @file libcanard_v0.h
 *
 * Header to include Libcanard v0 source files and printing functions.
 *
 * @author Joe DeFrance
 */


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

#include <Arduino.h>

//------------------------------------------------------------------------------

void printCanardFrame(CanardCANFrame* frame);

void printCanardInstance(CanardInstance* ins);

void printCanardRxTransfer(CanardRxTransfer* rxt);

void printCanardPoolAllocatorStatistics(CanardPoolAllocatorStatistics* stats);

//------------------------------------------------------------------------------

#endif // LIBCANARD_VERSION
