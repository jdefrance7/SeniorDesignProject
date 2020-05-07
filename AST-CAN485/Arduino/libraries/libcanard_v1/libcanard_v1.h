/**
 * @file libcanard_v0.h
 *
 * Header to include Libcanard v1 source files and printing functions.
 *
 * @author Joe DeFrance
 */

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

void printCanardFrame(CanardFrame* frame);

void printCanardTransfer(CanardTransfer* transfer);

void printCanardInstance(CanardInstance* ins);

//------------------------------------------------------------------------------

#endif // LIBCANARD_VERSION
