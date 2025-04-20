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

void PushingToBack() {
  printf("Running PushBackVector()…\n");
  Vector* vec = CreateVector();
  int expected = 42;
  PushBackVector(vec, &expected);

  int* result = PeekVector(vec, 0);
  assert(*result == expected);

  DestroyVector(vec);
}
