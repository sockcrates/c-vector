#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void CreatingEmptyVector() {
  printf("Running CreatingEmptyVector()…\n");
  Vector* vec = CreateVector();
  assert(vec != NULL);
  void* data = PeekVector(vec, 0);
  assert(data == NULL);
}
