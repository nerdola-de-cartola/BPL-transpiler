#ifndef CONDITIONS_H
#define CONDITIONS_H

#pragma once

#include <string.h>

#include "../common/common.h"
#include "../variables/variables.h"
#include "../assignment/assignment.h"

extern int IF_INDEX;

void ifStatement();

void verifyIfStatement(char c1, char c2, int index);

#endif
