#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector {
  size_t element_size;
  size_t size;
  size_t capacity;
  void* data;
} Vector;

Vector* CreateVector(size_t element_size);

void DestroyVector(Vector* vector);

void InsertVector(Vector* vector, size_t pos, void* data);

void* PeekVector(Vector* vector, size_t index);

void PushBackVector(Vector* vector, void* data);

void PushFrontVector(Vector* vector, void* data);

#endif
