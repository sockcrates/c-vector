#include "vector.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 10u

static bool MultiplySize(size_t left, size_t right, size_t *result) {
  if (left != 0 && right > SIZE_MAX / left)
    return false;
  *result = left * right;
  return true;
}
static bool AddSize(size_t left, size_t right, size_t *result) {
  if (right > SIZE_MAX - left)
    return false;
  *result = left + right;
  return true;
}
static unsigned char *ElementAt(Vector *vector, size_t index) {
  return (unsigned char *)vector->data + index * vector->element_size;
}
static const unsigned char *ConstElementAt(const Vector *vector, size_t index) {
  return (const unsigned char *)vector->data + index * vector->element_size;
}
static bool AllocatorIsValid(const VectorAllocator *allocator) {
  return allocator != NULL && allocator->malloc != NULL &&
         allocator->realloc != NULL && allocator->free != NULL;
}
static bool PointsIntoVector(const Vector *vector, const void *pointer) {
  size_t bytes;
  uintptr_t start, candidate;
  if (pointer == NULL || vector->data == NULL ||
      !MultiplySize(vector->size, vector->element_size, &bytes))
    return false;
  start = (uintptr_t)vector->data;
  candidate = (uintptr_t)pointer;
  return candidate >= start && candidate - start < bytes;
}
static VectorResult StageElementIfNeeded(Vector *vector, const void *element,
                                         bool needed, unsigned char **staged) {
  *staged = NULL;
  if (!needed || !PointsIntoVector(vector, element))
    return VECTOR_SUCCESS;
  *staged = vector->allocator.malloc(vector->element_size);
  if (*staged == NULL)
    return VECTOR_ALLOCATION_FAILURE;
  memcpy(*staged, element, vector->element_size);
  return VECTOR_SUCCESS;
}
static VectorResult ReserveInternal(Vector *vector, size_t capacity) {
  size_t bytes;
  void *new_data;
  if (capacity <= vector->capacity)
    return VECTOR_SUCCESS;
  if (!MultiplySize(capacity, vector->element_size, &bytes))
    return VECTOR_OVERFLOW;
  new_data = vector->allocator.realloc(vector->data, bytes);
  if (new_data == NULL)
    return VECTOR_ALLOCATION_FAILURE;
  vector->data = new_data;
  vector->capacity = capacity;
  return VECTOR_SUCCESS;
}
static VectorResult EnsureCapacity(Vector *vector, size_t needed) {
  size_t capacity = vector->capacity;
  if (needed <= capacity)
    return VECTOR_SUCCESS;
  if (capacity == 0)
    capacity = 1;
  while (capacity < needed) {
    if (capacity > SIZE_MAX / 2) {
      capacity = needed;
      break;
    }
    capacity *= 2;
  }
  return ReserveInternal(vector, capacity);
}

Vector *CreateVector(size_t element_size) {
  const VectorAllocator allocator = {malloc, realloc, free};
  return CreateVectorWithAllocator(element_size, &allocator);
}
Vector *CreateVectorWithAllocator(size_t element_size,
                                  const VectorAllocator *allocator) {
  Vector *vector;
  size_t bytes;
  if (element_size == 0 || !AllocatorIsValid(allocator) ||
      !MultiplySize(VECTOR_INITIAL_CAPACITY, element_size, &bytes))
    return NULL;
  vector = allocator->malloc(sizeof(*vector));
  if (vector == NULL)
    return NULL;
  vector->data = allocator->malloc(bytes);
  if (vector->data == NULL) {
    allocator->free(vector);
    return NULL;
  }
  vector->element_size = element_size;
  vector->size = 0;
  vector->capacity = VECTOR_INITIAL_CAPACITY;
  vector->allocator = *allocator;
  return vector;
}
void DestroyVector(Vector *vector) {
  if (vector != NULL) {
    vector->allocator.free(vector->data);
    vector->allocator.free(vector);
  }
}
VectorResult ReserveVector(Vector *vector, size_t capacity) {
  return vector == NULL ? VECTOR_NULL_INPUT : ReserveInternal(vector, capacity);
}
VectorResult PushBackVector(Vector *vector, const void *element) {
  size_t new_size;
  unsigned char *staged;
  VectorResult result;
  if (vector == NULL || element == NULL)
    return VECTOR_NULL_INPUT;
  if (!AddSize(vector->size, 1, &new_size))
    return VECTOR_OVERFLOW;
  result = StageElementIfNeeded(vector, element, new_size > vector->capacity,
                                &staged);
  if (result != VECTOR_SUCCESS)
    return result;
  result = EnsureCapacity(vector, new_size);
  if (result == VECTOR_SUCCESS) {
    memcpy(ElementAt(vector, vector->size), staged != NULL ? staged : element,
           vector->element_size);
    vector->size = new_size;
  }
  if (staged != NULL)
    vector->allocator.free(staged);
  return result;
}
VectorResult InsertVector(Vector *vector, size_t index, const void *element) {
  size_t new_size, move_bytes;
  unsigned char *staged;
  VectorResult result;
  if (vector == NULL || element == NULL)
    return VECTOR_NULL_INPUT;
  if (index > vector->size)
    return VECTOR_INVALID_INDEX;
  if (!AddSize(vector->size, 1, &new_size) ||
      !MultiplySize(vector->size - index, vector->element_size, &move_bytes))
    return VECTOR_OVERFLOW;
  result = StageElementIfNeeded(vector, element, true, &staged);
  if (result != VECTOR_SUCCESS)
    return result;
  result = EnsureCapacity(vector, new_size);
  if (result == VECTOR_SUCCESS) {
    unsigned char *at = ElementAt(vector, index);
    memmove(at + vector->element_size, at, move_bytes);
    memcpy(at, staged != NULL ? staged : element, vector->element_size);
    vector->size = new_size;
  }
  if (staged != NULL)
    vector->allocator.free(staged);
  return result;
}
VectorResult PushFrontVector(Vector *vector, const void *element) {
  return InsertVector(vector, 0, element);
}
VectorResult RemoveAtIndexVector(Vector *vector, size_t index) {
  size_t move_bytes;
  unsigned char *at;
  if (vector == NULL)
    return VECTOR_NULL_INPUT;
  if (index >= vector->size)
    return VECTOR_INVALID_INDEX;
  if (!MultiplySize(vector->size - index - 1, vector->element_size,
                    &move_bytes))
    return VECTOR_OVERFLOW;
  at = ElementAt(vector, index);
  memmove(at, at + vector->element_size, move_bytes);
  --vector->size;
  return VECTOR_SUCCESS;
}
VectorResult PopBackVector(Vector *vector, void *out_element) {
  if (vector == NULL || out_element == NULL)
    return VECTOR_NULL_INPUT;
  if (vector->size == 0)
    return VECTOR_INVALID_INDEX;
  memcpy(out_element, ElementAt(vector, vector->size - 1),
         vector->element_size);
  --vector->size;
  return VECTOR_SUCCESS;
}
VectorResult PopFrontVector(Vector *vector, void *out_element) {
  if (vector == NULL || out_element == NULL)
    return VECTOR_NULL_INPUT;
  if (vector->size == 0)
    return VECTOR_INVALID_INDEX;
  memcpy(out_element, vector->data, vector->element_size);
  return RemoveAtIndexVector(vector, 0);
}
VectorResult ResizeVector(Vector *vector, size_t size, const void *fill_value) {
  unsigned char *staged;
  VectorResult result;
  if (vector == NULL)
    return VECTOR_NULL_INPUT;
  if (size <= vector->size) {
    vector->size = size;
    return VECTOR_SUCCESS;
  }
  if (fill_value == NULL)
    return VECTOR_NULL_INPUT;
  result = StageElementIfNeeded(vector, fill_value, size > vector->capacity,
                                &staged);
  if (result != VECTOR_SUCCESS)
    return result;
  result = EnsureCapacity(vector, size);
  if (result == VECTOR_SUCCESS) {
    const void *fill = staged != NULL ? staged : fill_value;
    for (size_t i = vector->size; i < size; ++i)
      memcpy(ElementAt(vector, i), fill, vector->element_size);
    vector->size = size;
  }
  if (staged != NULL)
    vector->allocator.free(staged);
  return result;
}
VectorResult AppendVector(Vector *destination, const Vector *source) {
  size_t new_size, source_bytes;
  VectorResult result;
  if (destination == NULL || source == NULL)
    return VECTOR_NULL_INPUT;
  if (destination->element_size != source->element_size)
    return VECTOR_INCOMPATIBLE_ELEMENT_SIZES;
  if (!AddSize(destination->size, source->size, &new_size) ||
      !MultiplySize(source->size, source->element_size, &source_bytes))
    return VECTOR_OVERFLOW;
  result = EnsureCapacity(destination, new_size);
  if (result != VECTOR_SUCCESS)
    return result;
  memmove(ElementAt(destination, destination->size), source->data,
          source_bytes);
  destination->size = new_size;
  return VECTOR_SUCCESS;
}
void *GetVectorElement(Vector *vector, size_t index) {
  return vector != NULL && index < vector->size ? ElementAt(vector, index)
                                                : NULL;
}
const void *GetConstVectorElement(const Vector *vector, size_t index) {
  return vector != NULL && index < vector->size ? ConstElementAt(vector, index)
                                                : NULL;
}
void *GetFrontVector(Vector *vector) { return GetVectorElement(vector, 0); }
const void *GetConstFrontVector(const Vector *vector) {
  return GetConstVectorElement(vector, 0);
}
void *GetBackVector(Vector *vector) {
  return vector != NULL && vector->size != 0
             ? ElementAt(vector, vector->size - 1)
             : NULL;
}
const void *GetConstBackVector(const Vector *vector) {
  return vector != NULL && vector->size != 0
             ? ConstElementAt(vector, vector->size - 1)
             : NULL;
}
size_t GetVectorSize(const Vector *vector) {
  return vector == NULL ? 0 : vector->size;
}
size_t GetVectorCapacity(const Vector *vector) {
  return vector == NULL ? 0 : vector->capacity;
}
