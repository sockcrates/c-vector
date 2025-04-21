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

  int* resultA = PeekVector(vec, 0);
  assert(*resultA == a);
  int* resultB = PeekVector(vec, 1);
  assert(*resultB == b);
  int* resultC = PeekVector(vec, 2);
  assert(*resultC == c);

  DestroyVector(vec);
}

void PushingToFront() {
  printf("Running PushingToFront()…\n");
  Vector* vec = CreateVector();
  int a = 100;
  PushFrontVector(vec, &a);

  int* resultA = PeekVector(vec, 0);
  assert(*resultA == a);

  int b = 25;
  PushFrontVector(vec, &b);
  int* resultB = PeekVector(vec, 0);
  assert(*resultB == b);

  int c = 16;
  PushFrontVector(vec, &c);
  int* resultC = PeekVector(vec, 0);
  assert(*resultC == c);

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

  int* resultA = PeekVector(vec, 0);
  assert(*resultA == a);
  int* resultB = PeekVector(vec, 1);
  assert(*resultB == b);
  int* resultE = PeekVector(vec, 2);
  assert(*resultE == e);
  int* resultC = PeekVector(vec, 3);
  assert(*resultC == c);
  int* resultD = PeekVector(vec, 4);
  assert(*resultD == d);

  DestroyVector(vec);
}
