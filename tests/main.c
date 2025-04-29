#include <stdio.h>

#include "vector_test.h"

int main(void) {
  printf("Running test suite…\n");
  EmptyVector();
  PushingToBack();
  PushingToFront();
  InsertingVector();
  RemovingAtIndex();
  PoppingBack();
  printf("✅ All tests passed!\n");
  return 0;
}
