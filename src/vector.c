#include "vector.h"

#include <stdlib.h>

struct Vector {
  void** data;
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

void DestroyVector(Vector* vector) {
  if (vector != NULL) {
    free(vector->data);
    free(vector);
  }
}

void* PeekVector(Vector* vector, size_t index) {
  if (index < 0 || index >= vector->size) {
    return NULL;
  }
  return vector->data[index];
}

void PushBackVector(Vector* vector, void* data) {
  if (vector->size == vector->capacity) {
    size_t new_capacity = vector->capacity ? vector->capacity * 2 : 1;
    void** new_data = realloc(vector->data, new_capacity * sizeof(*new_data));
    if (!new_data) {
      return;
    }
    vector->data = new_data;
    vector->capacity = new_capacity;
  }
  vector->data[vector->size++] = data;
}
