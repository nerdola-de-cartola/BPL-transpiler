#ifndef ARRAY_H
#define ARRAY_H

#include "common.h"
#include "variables.h"
#include "registers.h"
#include "functions.h"

void verifyArrayAccess(int r, char c1, char c2, char c3);

void arrayAccess();

void arrayAccessGet(Register *r, char type, int index);

void arrayAccessSet(Register *r, char type, int index);

#endif