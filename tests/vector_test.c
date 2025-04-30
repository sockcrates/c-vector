#include "vector.h"

#include <assert.h>
#include <stdio.h>

void EmptyVector() {
  printf("Running EmptyVector()…\n");
  Vector* vec = CreateVector(sizeof(int));

  assert(vec != NULL);

  DestroyVector(vec);
}

void InvalidSizedElementVector() {
  printf("Running InvalidZeroSizedElementVector()…\n");
  Vector* zero_vec = CreateVector(0);

  assert(zero_vec == NULL);

  DestroyVector(zero_vec);

  Vector* negative_vec = CreateVector(0);

  assert(negative_vec == NULL);

  DestroyVector(negative_vec);
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
  int a = 11;
  int b = 256;
  int c = 3018;
  int d = 2;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  int e = 5;
  InsertVector(vec, 2, &e);
  int f = 8;
  InsertVector(vec, 5, &f);

  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == b);
  assert(data[2] == e);
  assert(data[3] == c);
  assert(data[4] == d);
  assert(data[5] == f);

  DestroyVector(vec);
}

void InsertingVectorInvalidIndex() {
  printf("Running InsertingVectorInvalidIndex()…\n");
  Vector* vec = CreateVector(sizeof(int));
  int a = 988517;
  int b = 6;
  int c = 38;
  int d = 87;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  InsertVector(vec, -1, &(int){ 2 });

  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == b);
  assert(data[2] == c);
  assert(data[3] == d);

  InsertVector(vec, 5, &(int){ 29929 });
  assert(vec->size == 4);

  DestroyVector(vec);
}

void RemovingAtIndex() {
  printf("Running RemovingAtIndex()…\n");
  Vector* vec = CreateVector(sizeof(int));
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  RemoveAtIndexVector(vec, 1);
  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == c);
  assert(data[2] == d);

  DestroyVector(vec);
}

void RemovingAtInvalidIndex() {
  printf("Running RemovingAtInvalidIndex()…\n");
  Vector* vec = CreateVector(sizeof(int));
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  RemoveAtIndexVector(vec, -1);
  RemoveAtIndexVector(vec, 4);
  int* data = (int*)vec->data;
  assert(data[0] == a);
  assert(data[1] == b);
  assert(data[2] == c);
  assert(data[3] == d);

  DestroyVector(vec);
}

void PoppingBack() {
  printf("Running PoppingBack()…\n");
  Vector* vec = CreateVector(sizeof(int));

  assert(PopBackVector(vec) == NULL);

  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  int* value_d = PopBackVector(vec);
  assert(*value_d == d);
  int* value_c = PopBackVector(vec);
  assert(*value_c == c);
  int* value_b = PopBackVector(vec);
  assert(*value_b == b);
  int* value_a = PopBackVector(vec);
  assert(*value_a == a);

  DestroyVector(vec);
}

void PoppingFront() {
  printf("Running PoppingFront()…\n");
  Vector* vec = CreateVector(sizeof(int));

  assert(PopFrontVector(vec) == NULL);

  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  PushBackVector(vec, &a);
  PushBackVector(vec, &b);
  PushBackVector(vec, &c);
  PushBackVector(vec, &d);

  int* value_a = PopFrontVector(vec);
  assert(*value_a == a);
  int* value_b = PopFrontVector(vec);
  assert(*value_b == b);
  int* value_c = PopFrontVector(vec);
  assert(*value_c == c);
  int* value_d = PopFrontVector(vec);
  assert(*value_d == d);

  DestroyVector(vec);
}

void MergingVectors() {
  printf("Running MergingVectors()…\n");
  Vector* foo = CreateVector(sizeof(char));
  PushBackVector(foo, "F");
  PushBackVector(foo, "o");
  PushBackVector(foo, "o");
  Vector* bar_baz = CreateVector(sizeof(char));
  PushBackVector(bar_baz, &(char){ 'B' });
  PushBackVector(bar_baz, &(char){ 'a' });
  PushBackVector(bar_baz, &(char){ 'r' });
  PushBackVector(bar_baz, &(char){ 'B' });
  PushBackVector(bar_baz, &(char){ 'a' });
  PushBackVector(bar_baz, &(char){ 'z' });

  char* expected = "FooBarBaz";
  MergeVector(foo, bar_baz);
  char* foo_bar_baz = (char*)foo->data;
  for (size_t i = 0; i < foo->size; ++i) {
    assert(expected[i] == foo_bar_baz[i]);
  }
  assert(foo->size == 9);

  DestroyVector(foo);
  DestroyVector(bar_baz);
}

void MergingIncompatibleVectors() {
  Vector* big = CreateVector(sizeof(long));
  PushBackVector(big, &(long){ 1 });
  Vector* small = CreateVector(sizeof(int));
  PushBackVector(small, &(int){ 1 });

  MergeVector(big, small);
  assert(big->size == 1);
  assert(small->size == 1);

  DestroyVector(big);
  DestroyVector(small);
}
