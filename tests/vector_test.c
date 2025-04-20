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
  int a = 42;
  PushBackVector(vec, &a);
  int b = 42;
  PushBackVector(vec, &b);
  int c = 42;
  PushBackVector(vec, &c);

  int* resultA = PeekVector(vec, 0);
  assert(*resultA == a);
  int* resultB = PeekVector(vec, 1);
  assert(*resultB == b);
  int* resultC = PeekVector(vec, 2);
  assert(*resultC == c);

  DestroyVector(vec);
}
