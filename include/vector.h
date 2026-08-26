#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef void *(*VectorMalloc)(size_t size);
typedef void *(*VectorRealloc)(void *pointer, size_t size);
typedef void (*VectorFree)(void *pointer);

typedef struct VectorAllocator {
  VectorMalloc malloc;
  VectorRealloc realloc;
  VectorFree free;
} VectorAllocator;

typedef enum VectorResult {
  VECTOR_SUCCESS = 0,
  VECTOR_NULL_INPUT,
  VECTOR_INVALID_INDEX,
  VECTOR_ALLOCATION_FAILURE,
  VECTOR_OVERFLOW,
  VECTOR_INCOMPATIBLE_ELEMENT_SIZES,
  VECTOR_INVALID_ALLOCATOR
} VectorResult;

typedef struct Vector {
  size_t element_size;
  size_t size;
  size_t capacity;
  void *data;
  VectorAllocator allocator;
} Vector;

Vector *CreateVector(size_t element_size);
Vector *CreateVectorWithAllocator(size_t element_size,
                                  const VectorAllocator *allocator);
void DestroyVector(Vector *vector);

VectorResult ReserveVector(Vector *vector, size_t capacity);
VectorResult ResizeVector(Vector *vector, size_t size, const void *fill_value);
VectorResult InsertVector(Vector *vector, size_t index, const void *element);
VectorResult PushBackVector(Vector *vector, const void *element);
VectorResult PushFrontVector(Vector *vector, const void *element);
VectorResult RemoveAtIndexVector(Vector *vector, size_t index);
VectorResult PopBackVector(Vector *vector, void *out_element);
VectorResult PopFrontVector(Vector *vector, void *out_element);
VectorResult AppendVector(Vector *destination, const Vector *source);

/* These pointers are invalidated by operations that move or reallocate data. */
void *GetVectorElement(Vector *vector, size_t index);
const void *GetConstVectorElement(const Vector *vector, size_t index);
void *GetFrontVector(Vector *vector);
const void *GetConstFrontVector(const Vector *vector);
void *GetBackVector(Vector *vector);
const void *GetConstBackVector(const Vector *vector);
size_t GetVectorSize(const Vector *vector);
size_t GetVectorCapacity(const Vector *vector);

#endif
