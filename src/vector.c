#include "vector.h"

#include <stdlib.h>

struct Vector {
  void* data;
  size_t size;
  size_t capacity;
};

Vector* CreateVector() {
  Vector* vec = malloc(sizeof(Vector));
  if (vec == NULL) {
    return NULL;
  }
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
  return vec;
}

void* PeekVector(Vector *vector, int index) {
  if (index < 0 || index >= vector->size) {
    return NULL;
  }
  return &vector->data[index];
}
