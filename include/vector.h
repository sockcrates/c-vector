#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector Vector;

Vector* CreateVector();

void* PeekVector(Vector* vector, size_t index);

#endif
