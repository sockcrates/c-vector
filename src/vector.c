#include "vector.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Vector* CreateVector(size_t element_size) {
  if (element_size <= 0) {
    return NULL;
  }
  Vector* vec = malloc(sizeof(Vector));
  if (vec == NULL) {
    return NULL;
  }
  vec->element_size = element_size;
  vec->size = 0;
  vec->capacity = 10;
  vec->data = malloc(vec->capacity * vec->element_size);
  if (vec->data == NULL) {
    return NULL;
  }
  return vec;
}

void DestroyVector(Vector* vector) {
  if (vector != NULL) {
    free(vector->data);
    free(vector);
  }
}

bool ExpandVector(Vector* vector) {
  size_t new_capacity = vector->capacity ? vector->capacity * 2 : 1;
  void* new_data = realloc(vector->data, new_capacity * vector->element_size);
  if (new_data == NULL) {
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

void InsertVector(Vector* vector, size_t index, void* data) {
  if (index < 0 || index > vector->size) {
    return;
  }
  if (vector->size == vector->capacity) {
    if (!ExpandVector(vector)) {
      return;
    }
  }
  void* dest = vector->data + (index * vector->element_size);
  void* move_dest = dest + vector->element_size;
  size_t move_size = (vector->size - index) * vector->element_size;
  memmove(move_dest, dest, move_size);
  memcpy(dest, data, vector->element_size);
  vector->size++;
}

void PushFrontVector(Vector* vector, void* data) {
  InsertVector(vector, 0, data);
}

void RemoveAtIndexVector(Vector* vector, size_t index) {
  if (index < 0 || index > vector->size - 1) {
    return;
  }
  void* dest = vector->data + (index * vector->element_size);
  size_t move_size = (vector->size - index - 1) * vector->element_size;
  memmove(dest, dest + vector->element_size, move_size);
  vector->size--;
}

void* PopBackVector(Vector* vector) {
  if (vector->size == 0) {
    return NULL;
  }
  size_t last_element = vector->size - 1;
  void* result = malloc(vector->element_size);
  memcpy(result, vector->data + (vector->element_size * last_element),
         vector->element_size);
  vector->size--;
  return result;
}

void* PopFrontVector(Vector* vector) {
  if (vector->size == 0) {
    return NULL;
  }
  void* result = malloc(vector->element_size);
  memcpy(result, vector->data, vector->element_size);
  RemoveAtIndexVector(vector, 0);
  return result;
}

void MergeVector(Vector* destination, Vector* source) {
  if (destination->element_size != source->element_size) {
    return;
  }
  size_t free_space = destination->capacity - destination->size;
  if (free_space < source->size) {
    size_t new_capacity = destination->capacity + source->size;
    void* new_data = realloc(destination->data, new_capacity);
    destination->data = new_data;
  }
  void* dest =
      destination->data + (destination->size * destination->element_size);
  memmove(dest, source->data, source->size);
  destination->size += source->size;
}
