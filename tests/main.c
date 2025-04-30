#include <stdio.h>

#include "vector_test.h"

int main(void) {
  printf("Running test suite…\n");
  EmptyVector();
  InvalidSizedElementVector();
  PushingToBack();
  PushingToFront();
  InsertingVector();
  InsertingVectorInvalidIndex();
  RemovingAtIndex();
  RemovingAtInvalidIndex();
  PoppingBack();
  PoppingFront();
  MergingVectors();
  MergingIncompatibleVectors();
  printf("✅ All tests passed!\n");
  return 0;
}
