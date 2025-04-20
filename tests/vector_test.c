#include "vector.h"

#include <assert.h>
#include <stdio.h>

void EmptyVector() {
  printf("Running CreatingEmptyVector()…\n");
  Vector* vec = CreateVector();

  assert(vec != NULL);
  void* data = PeekVector(vec, 0);

  assert(data == NULL);
  DestroyVector(vec);
}
