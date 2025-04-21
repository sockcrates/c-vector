#include "vector.h"

#include <stdbool.h>
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

bool ExpandVector(Vector* vector) {
  size_t new_capacity = vector->capacity ? vector->capacity * 2 : 1;
  void** new_data = realloc(vector->data, new_capacity * sizeof(*new_data));
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
  vector->data[vector->size++] = data;
}

void InsertVector(Vector* vector, size_t pos, void* data) {
  if (vector->size == vector->capacity) {
    if (!ExpandVector(vector)) {
      return;
    }
  }
  for (size_t i = vector->size; i > pos; --i) {
    vector->data[i] = vector->data[i - 1];
  }
  vector->data[pos] = data;
  vector->size++;
}

void PushFrontVector(Vector* vector, void* data) {
  InsertVector(vector, 0, data);
}
