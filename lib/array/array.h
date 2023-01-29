#ifndef ARRAY_H
#define ARRAY_H

#include "../common/common.h"
#include "../variables/variables.h"
#include "../registers/registers.h"
#include "../functions/functions.h"

void verifyArrayAccess(int r, char c1, char c2, char c3);

void arrayAccess();

void arrayAccessGet(Register *r, char type, int index);

void arrayAccessSet(Register *r, char type, int index);

#endif