#include "vector.h"

#include <assert.h>
#include <stdio.h>

void EmptyVector() {
  printf("Running EmptyVector()…\n");
  Vector* vec = CreateVector(sizeof(int));

  assert(vec != NULL);

  DestroyVector(vec);
}

void PushingToBack() {
  printf("Running PushingToBack()…\n");
  Vector* vec = CreateVector(sizeof(int));
  int a = 42;
  PushBackVector(vec, &a);
  int b = 21;
  PushBackVector(vec, &b);
  int c = 11;
  PushBackVector(vec, &c);

  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == b);
  assert(data[2] == c);

  DestroyVector(vec);
}

void PushingToFront() {
  printf("Running PushingToFront()…\n");
  Vector* vec = CreateVector(sizeof(int));
  int a = 100;
  PushFrontVector(vec, &a);

  int* data = (int*)vec->data;
  assert(data[0] == a);

  int b = 25;
  PushFrontVector(vec, &b);
  assert(data[0] == b);

  int c = 16;
  PushFrontVector(vec, &c);
  assert(data[0] == c);

  DestroyVector(vec);
}

void InsertingVector() {
  printf("Running InsertingVector()…\n");
  Vector* vec = CreateVector(sizeof(int));
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

  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == b);
  assert(data[2] == e);
  assert(data[3] == c);
  assert(data[4] == d);

  DestroyVector(vec);
}
