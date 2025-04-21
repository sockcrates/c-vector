#include <stdio.h>

#include "vector_test.h"

int main(void) {
  printf("Running test suite…\n");
  EmptyVector();
  PushingToBack();
  PushingToFront();
  InsertingVector();
  printf("✅ All tests passed!\n");
  return 0;
}
