#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector Vector;

Vector* CreateVector();

void DestroyVector(Vector* vector);

void* PeekVector(Vector* vector, size_t index);

void PushBackVector(Vector* vector, void* data);

void PushFrontVector(Vector* vector, void* data);

#endif
