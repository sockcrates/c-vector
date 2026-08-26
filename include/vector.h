#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

/** @file vector.h
 * @brief Generic, contiguous vector operations.
 */

/** Allocates `size` bytes for a vector. */
typedef void *(*VectorMalloc)(size_t size);
/** Resizes a vector allocation to `size` bytes. */
typedef void *(*VectorRealloc)(void *pointer, size_t size);
/** Releases a vector allocation. */
typedef void (*VectorFree)(void *pointer);

/**
 * Memory functions used by a Vector.
 *
 * Each function must have the same semantics as its standard-library
 * counterpart. The allocator structure need only remain valid for the create
 * call because the vector copies it, but its function pointers must remain
 * callable until the vector is destroyed.
 */
typedef struct VectorAllocator {
  /** Function used to allocate new storage. */
  VectorMalloc malloc;
  /** Function used to resize existing storage. */
  VectorRealloc realloc;
  /** Function used to release storage. */
  VectorFree free;
} VectorAllocator;

/** Status used by vector operations. */
typedef enum VectorResult {
  /** The operation completed successfully. */
  VECTOR_SUCCESS = 0,
  /** A required pointer argument was null. */
  VECTOR_NULL_INPUT,
  /** An index was outside the operation's valid range. */
  VECTOR_INVALID_INDEX,
  /** The allocator could not provide the requested storage. */
  VECTOR_ALLOCATION_FAILURE,
  /** A size or byte-count calculation overflowed `size_t`. */
  VECTOR_OVERFLOW,
  /** Two vectors have different element sizes and cannot be appended. */
  VECTOR_INCOMPATIBLE_ELEMENT_SIZES,
  /** Reserved for an invalid allocator; creation currently returns `NULL`. */
  VECTOR_INVALID_ALLOCATOR
} VectorResult;

/**
 * A contiguous, owning collection of fixed-size elements.
 *
 * Elements are copied as `element_size` bytes; the vector neither constructs
 * nor destroys them. Callers should use the functions below to preserve the
 * vector invariants. The fields are exposed for inspection but must not be
 * modified directly.
 */
typedef struct Vector {
  /** Size in bytes of every element. */
  size_t element_size;
  /** Number of initialized elements. */
  size_t size;
  /** Number of elements that fit in the current allocation. */
  size_t capacity;
  /** Contiguous element storage. */
  void *data;
  /** Allocator that owns both this object and its element storage. */
  VectorAllocator allocator;
} Vector;

/**
 * Creates an empty vector using the standard memory allocator.
 *
 * @param element_size Size, in bytes, of one element; must be nonzero.
 * @return A new vector, or `NULL` if `element_size` is zero, its initial
 *     allocation overflows, or allocation fails.
 *
 * @par Example
 * @code{.c}
 * Vector *numbers = CreateVector(sizeof(int));
 * int value = 42;
 *
 * if (numbers != NULL && PushBackVector(numbers, &value) == VECTOR_SUCCESS) {
 *   const int *first = GetConstFrontVector(numbers);
 *   // use *first
 * }
 * DestroyVector(numbers);
 * @endcode
 */
Vector *CreateVector(size_t element_size);
/**
 * Creates an empty vector using `allocator`.
 *
 * @param element_size Size, in bytes, of one element; must be nonzero.
 * @param allocator Complete allocator used for the vector's lifetime.
 * @return A new vector, or `NULL` for invalid input, overflow, or allocation
 *     failure.
 */
Vector *CreateVectorWithAllocator(size_t element_size,
                                  const VectorAllocator *allocator);
/**
 * Releases a vector and all of its element storage.
 *
 * @param vector Vector to destroy; may be `NULL`.
 */
void DestroyVector(Vector *vector);

/**
 * Ensures that a vector can hold at least `capacity` elements.
 *
 * @param vector Vector to reserve storage for.
 * @param capacity Minimum desired capacity.
 * @return `VECTOR_NULL_INPUT`, `VECTOR_OVERFLOW`, or
 *     `VECTOR_ALLOCATION_FAILURE` on failure; otherwise `VECTOR_SUCCESS`.
 *     This function never reduces capacity and preserves all elements.
 */
VectorResult ReserveVector(Vector *vector, size_t capacity);
/**
 * Changes a vector's size.
 *
 * Shrinking discards trailing elements. Growing copies `fill_value` into each
 * new element.
 *
 * @param vector Vector to resize.
 * @param size New element count.
 * @param fill_value Pointer to at least `element_size` readable bytes copied
 *     into new elements when growing; may be `NULL` only when not growing.
 * @return A VectorResult describing the result.
 */
VectorResult ResizeVector(Vector *vector, size_t size, const void *fill_value);
/**
 * Inserts a copy of `element` before `index`.
 *
 * `index` may equal the current size. `element` may point to an existing
 * element in `vector`.
 *
 * @param vector Destination vector.
 * @param index Insertion position, from zero through the current size.
 * @param element Pointer to at least `element_size` readable bytes to insert.
 * @return `VECTOR_INVALID_INDEX` when `index` exceeds the size, or another
 *     VectorResult describing the result.
 */
VectorResult InsertVector(Vector *vector, size_t index, const void *element);
/**
 * Appends a copy of `element`.
 *
 * @param vector Destination vector.
 * @param element Pointer to at least `element_size` readable bytes to append;
 *     it may reference an existing element.
 * @return A VectorResult describing the result.
 */
VectorResult PushBackVector(Vector *vector, const void *element);
/**
 * Prepends a copy of `element`.
 *
 * @param vector Destination vector.
 * @param element Pointer to at least `element_size` readable bytes to prepend;
 *     it may reference an existing element.
 * @return A VectorResult describing the result.
 */
VectorResult PushFrontVector(Vector *vector, const void *element);
/**
 * Removes the element at `index` and shifts subsequent elements left.
 *
 * @param vector Vector to remove from.
 * @param index Index of the element to remove.
 * @return `VECTOR_INVALID_INDEX` if `index` is not an element index, or a
 *     VectorResult describing the result.
 */
VectorResult RemoveAtIndexVector(Vector *vector, size_t index);
/**
 * Copies and removes the last element.
 *
 * @param vector Vector to pop from.
 * @param out_element Writable destination for at least `element_size` bytes;
 *     must not be `NULL` or overlap vector storage.
 * @return `VECTOR_INVALID_INDEX` if the vector is empty, or a VectorResult.
 */
VectorResult PopBackVector(Vector *vector, void *out_element);
/**
 * Copies and removes the first element.
 *
 * @param vector Vector to pop from.
 * @param out_element Writable destination for at least `element_size` bytes;
 *     must not be `NULL` or overlap vector storage.
 * @return `VECTOR_INVALID_INDEX` if the vector is empty, or a VectorResult.
 */
VectorResult PopFrontVector(Vector *vector, void *out_element);
/**
 * Appends copies of all elements in `source` to `destination`.
 *
 * A vector may be appended to itself. The vectors must have the same element
 * size.
 *
 * @param destination Vector to extend.
 * @param source Vector whose elements are copied.
 * @return `VECTOR_INCOMPATIBLE_ELEMENT_SIZES` for differently sized elements,
 *     or another VectorResult describing the result.
 *
 * @par Example
 * @code{.c}
 * // Duplicates every existing element in the vector.
 * VectorResult result = AppendVector(vector, vector);
 * @endcode
 */
VectorResult AppendVector(Vector *destination, const Vector *source);

/**
 * Returns a pointer to an element.
 *
 * A reallocation invalidates all returned pointers. Insertion, removal, and a
 * shrinking resize can invalidate or change pointers to affected elements.
 *
 * @param vector Vector to inspect.
 * @param index Element index.
 * @return A mutable pointer to the element, or `NULL` for null input or an
 *     invalid index.
 */
void *GetVectorElement(Vector *vector, size_t index);
/**
 * Returns a read-only pointer to an element.
 *
 * A reallocation invalidates all returned pointers. Insertion, removal, and a
 * shrinking resize can invalidate or change pointers to affected elements.
 *
 * @param vector Vector to inspect.
 * @param index Element index.
 * @return A pointer to the element, or `NULL` for null input or an invalid
 *     index.
 */
const void *GetConstVectorElement(const Vector *vector, size_t index);
/**
 * Returns a mutable pointer to the first element.
 *
 * @param vector Vector to inspect.
 * @return The first element, or `NULL` if `vector` is null or empty.
 */
void *GetFrontVector(Vector *vector);
/**
 * Returns a read-only pointer to the first element.
 *
 * @param vector Vector to inspect.
 * @return The first element, or `NULL` if `vector` is null or empty.
 */
const void *GetConstFrontVector(const Vector *vector);
/**
 * Returns a mutable pointer to the last element.
 *
 * @param vector Vector to inspect.
 * @return The last element, or `NULL` if `vector` is null or empty.
 */
void *GetBackVector(Vector *vector);
/**
 * Returns a read-only pointer to the last element.
 *
 * @param vector Vector to inspect.
 * @return The last element, or `NULL` if `vector` is null or empty.
 */
const void *GetConstBackVector(const Vector *vector);
/**
 * Returns the number of elements in a vector.
 *
 * @param vector Vector to inspect.
 * @return The vector size, or zero when `vector` is `NULL`.
 */
size_t GetVectorSize(const Vector *vector);
/**
 * Returns the allocated capacity of a vector.
 *
 * @param vector Vector to inspect.
 * @return The vector capacity, or zero when `vector` is `NULL`.
 */
size_t GetVectorCapacity(const Vector *vector);

#endif
