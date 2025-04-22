#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Vector {
  size_t element_size;
  size_t size;
  size_t capacity;
  void* data;
};

Vector* CreateVector(size_t element_size) {
  Vector* vec = malloc(sizeof(Vector));
  if (vec == NULL) {
    return NULL;
  }
  vec->element_size = element_size;
  vec->size = 0;
  vec->capacity = 10;
  vec->data = malloc(vec->capacity * vec->element_size);
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
  return vector->data + (index * vector->element_size);
}

bool ExpandVector(Vector* vector) {
  size_t new_capacity = vector->capacity ? vector->capacity * 2 : 1;
  void* new_data = realloc(vector->data, new_capacity * vector->element_size);
  if (!new_data) {
    return false;
  }
  vector->data = new_data;
  vector->capacity = new_capacity;
  return true;
}

void PushBackVector(Vector* vector, void* data) {
  if (vector->size == vector->capacity) {
    if (!ExpandVector(vector)) {
      return;
    }
  }
  void* dest = vector->data + (vector->size * vector->element_size);
  memcpy(dest, data, vector->element_size);
  vector->size++;
}

void InsertVector(Vector* vector, size_t pos, void* data) {
  if (vector->size == vector->capacity) {
    if (!ExpandVector(vector)) {
      return;
    }
  }
  void* dest = vector->data + (pos * vector->element_size);
  void* move_dest = dest + vector->element_size;
  size_t move_size = (vector->size - pos) * vector->element_size;
  memmove(move_dest, dest, move_size);
  memcpy(dest, data, vector->element_size);
  vector->size++;
}

void PushFrontVector(Vector* vector, void* data) {
  InsertVector(vector, 0, data);
}
