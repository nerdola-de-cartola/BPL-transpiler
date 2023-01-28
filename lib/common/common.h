#ifndef COMMON_H
#define COMMON_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 256

extern FILE *F_SOURCE;
extern FILE *F_OUTPUT;
extern char BUFFER[MAX_LINE_SIZE];
extern int LINE_COUNT;

void remove_newline(char *ptr);

bool strInStr(char *string, char *substring);

bool charInStr(const char c, const char *str);

void error(const char *error_type);

char *readNewLine();

void beginFile();

#endif