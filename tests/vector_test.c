#include "vector.h"

#include <assert.h>
#include <stdio.h>

void EmptyVector() {
  printf("Running EmptyVector()…\n");
  Vector* vec = CreateVector();

  assert(vec != NULL);
  void* data = PeekVector(vec, 0);
  assert(data == NULL);

  DestroyVector(vec);
}

void PushingToBack() {
  printf("Running PushingToBack()…\n");
  Vector* vec = CreateVector();
  int a = 42;
  PushBackVector(vec, &a);
  int b = 21;
  PushBackVector(vec, &b);
  int c = 11;
  PushBackVector(vec, &c);

  int* result_a = PeekVector(vec, 0);
  assert(*result_a == a);
  int* result_b = PeekVector(vec, 1);
  assert(*result_b == b);
  int* result_c = PeekVector(vec, 2);
  assert(*result_c == c);

  DestroyVector(vec);
}

void PushingToFront() {
  printf("Running PushingToFront()…\n");
  Vector* vec = CreateVector();
  int a = 100;
  PushFrontVector(vec, &a);

  int* result_a = PeekVector(vec, 0);
  assert(*result_a == a);

  int b = 25;
  PushFrontVector(vec, &b);
  int* result_b = PeekVector(vec, 0);
  assert(*result_b == b);

  int c = 16;
  PushFrontVector(vec, &c);
  int* result_c = PeekVector(vec, 0);
  assert(*result_c == c);

  DestroyVector(vec);
}

void InsertingVector() {
  printf("Running InsertingVector()…\n");
  Vector* vec = CreateVector();
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);
  int e = 5;
  InsertVector(vec, 2, &e);

  int* result_a = PeekVector(vec, 0);
  assert(*result_a == a);
  int* result_b = PeekVector(vec, 1);
  assert(*result_b == b);
  int* result_e = PeekVector(vec, 2);
  assert(*result_e == e);
  int* result_c = PeekVector(vec, 3);
  assert(*result_c == c);
  int* result_d = PeekVector(vec, 4);
  assert(*result_d == d);
  void* outOfBounds = PeekVector(vec, 5);
  assert(outOfBounds == NULL);

  DestroyVector(vec);
}
