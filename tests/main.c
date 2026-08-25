#include <stdio.h>

#include "vector_test.h"

int main(void) {
  printf("Running test suite…\n");
  EmptyVector();
  InvalidSizedElementVector();
  PushingToBack();
  PushingToFront();
  InsertingVector();
  RemovingAtIndex();
  Popping();
  ResizeAndAccessors();
  AliasedMutations();
  AppendingVectors();
  AllocationFailures();
  printf("✅ All tests passed!\n");
  return 0;
}
