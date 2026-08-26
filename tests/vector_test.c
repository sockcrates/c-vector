#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pair {
  int first;
  int second;
} Pair;

typedef struct FailingAllocator {
  size_t calls;
  size_t fail_on;
  size_t frees;
} FailingAllocator;
static FailingAllocator allocator_state;
static void *TestMalloc(size_t bytes) {
  ++allocator_state.calls;
  return allocator_state.calls == allocator_state.fail_on ? NULL
                                                          : malloc(bytes);
}
static void *TestRealloc(void *pointer, size_t bytes) {
  ++allocator_state.calls;
  return allocator_state.calls == allocator_state.fail_on
             ? NULL
             : realloc(pointer, bytes);
}
static void TestFree(void *pointer) {
  ++allocator_state.frees;
  free(pointer);
}
static const VectorAllocator failing_allocator = {TestMalloc, TestRealloc,
                                                  TestFree};

static void CheckInts(const Vector *vector, const int *expected, size_t count) {
  assert(GetVectorSize(vector) == count);
  for (size_t i = 0; i < count; ++i)
    assert(*(const int *)GetConstVectorElement(vector, i) == expected[i]);
}

void EmptyVector(void) {
  Vector *vector = CreateVector(sizeof(int));
  assert(vector != NULL && GetVectorSize(vector) == 0 &&
         GetVectorCapacity(vector) == 10);
  assert(GetFrontVector(vector) == NULL && GetBackVector(vector) == NULL);
  DestroyVector(vector);
}
void InvalidSizedElementVector(void) {
  VectorAllocator incomplete = {malloc, NULL, free};
  assert(CreateVector(0) == NULL);
  assert(CreateVectorWithAllocator(sizeof(int), NULL) == NULL);
  assert(CreateVectorWithAllocator(sizeof(int), &incomplete) == NULL);
  assert(ReserveVector(NULL, 4) == VECTOR_NULL_INPUT);
}
void PushingToBack(void) {
  Vector *vector = CreateVector(sizeof(int));
  for (int i = 0; i < 15; ++i)
    assert(PushBackVector(vector, &i) == VECTOR_SUCCESS);
  for (size_t i = 0; i < 15; ++i)
    assert(*(int *)GetVectorElement(vector, i) == (int)i);
  assert(GetVectorCapacity(vector) >= 15);
  DestroyVector(vector);
}
void PushingToFront(void) {
  Vector *vector = CreateVector(sizeof(int));
  int a = 1, b = 2;
  assert(PushFrontVector(vector, &a) == VECTOR_SUCCESS);
  assert(PushFrontVector(vector, &b) == VECTOR_SUCCESS);
  {
    const int expected[] = {2, 1};
    CheckInts(vector, expected, 2);
  }
  DestroyVector(vector);
}
void InsertingVector(void) {
  Vector *vector = CreateVector(sizeof(int));
  int values[] = {1, 2, 4};
  int three = 3;
  for (size_t i = 0; i < 3; ++i)
    assert(PushBackVector(vector, &values[i]) == VECTOR_SUCCESS);
  assert(InsertVector(vector, 2, &three) == VECTOR_SUCCESS);
  {
    const int expected[] = {1, 2, 3, 4};
    CheckInts(vector, expected, 4);
  }
  assert(InsertVector(vector, 5, &three) == VECTOR_INVALID_INDEX);
  assert(InsertVector(NULL, 0, &three) == VECTOR_NULL_INPUT);
  DestroyVector(vector);
}
void RemovingAtIndex(void) {
  Vector *vector = CreateVector(sizeof(int));
  int values[] = {1, 2, 3};
  for (size_t i = 0; i < 3; ++i)
    PushBackVector(vector, &values[i]);
  assert(RemoveAtIndexVector(vector, 1) == VECTOR_SUCCESS);
  {
    const int expected[] = {1, 3};
    CheckInts(vector, expected, 2);
  }
  assert(RemoveAtIndexVector(vector, 2) == VECTOR_INVALID_INDEX);
  DestroyVector(vector);
}
void Popping(void) {
  Vector *vector = CreateVector(sizeof(int));
  int a = 1, b = 2, out = 0;
  assert(PopBackVector(vector, &out) == VECTOR_INVALID_INDEX);
  assert(PopBackVector(vector, NULL) == VECTOR_NULL_INPUT);
  PushBackVector(vector, &a);
  PushBackVector(vector, &b);
  assert(PopFrontVector(vector, &out) == VECTOR_SUCCESS && out == a);
  assert(PopBackVector(vector, &out) == VECTOR_SUCCESS && out == b);
  DestroyVector(vector);
}
void ResizeAndAccessors(void) {
  Vector *vector = CreateVector(sizeof(Pair));
  Pair pair = {7, 9};
  assert(ResizeVector(vector, 3, &pair) == VECTOR_SUCCESS);
  assert(((Pair *)GetFrontVector(vector))->second == 9);
  assert(((const Pair *)GetConstBackVector(vector))->first == 7);
  assert(ResizeVector(vector, 1, NULL) == VECTOR_SUCCESS);
  assert(ResizeVector(vector, 2, NULL) == VECTOR_NULL_INPUT);
  DestroyVector(vector);
}
void AliasedMutations(void) {
  Vector *vector = CreateVector(sizeof(int));
  for (int i = 0; i < 10; ++i)
    PushBackVector(vector, &i);
  assert(PushBackVector(vector, GetVectorElement(vector, 3)) == VECTOR_SUCCESS);
  assert(*(int *)GetBackVector(vector) == 3);
  assert(InsertVector(vector, 1, GetVectorElement(vector, 8)) ==
         VECTOR_SUCCESS);
  assert(*(int *)GetVectorElement(vector, 1) == 8);
  DestroyVector(vector);
}
void AppendingVectors(void) {
  Vector *left = CreateVector(sizeof(Pair));
  Vector *right = CreateVector(sizeof(Pair));
  Pair one = {1, 2}, two = {3, 4};
  PushBackVector(left, &one);
  PushBackVector(right, &two);
  assert(AppendVector(left, right) == VECTOR_SUCCESS);
  assert(((Pair *)GetBackVector(left))->first == 3);
  assert(AppendVector(left, left) == VECTOR_SUCCESS);
  assert(GetVectorSize(left) == 4);
  {
    Vector *incompatible = CreateVector(sizeof(int));
    assert(AppendVector(left, incompatible) ==
           VECTOR_INCOMPATIBLE_ELEMENT_SIZES);
    DestroyVector(incompatible);
  }
  DestroyVector(left);
  DestroyVector(right);
}
void AllocationFailures(void) {
  Vector *vector;
  int value = 5;
  allocator_state = (FailingAllocator){0, 3, 0};
  vector = CreateVectorWithAllocator(sizeof(int), &failing_allocator);
  assert(vector != NULL);
  for (int i = 0; i < 10; ++i)
    PushBackVector(vector, &i);
  assert(PushBackVector(vector, &value) == VECTOR_ALLOCATION_FAILURE);
  assert(GetVectorSize(vector) == 10 && *(int *)GetBackVector(vector) == 9);
  DestroyVector(vector);
  assert(allocator_state.frees >= 2);
}
