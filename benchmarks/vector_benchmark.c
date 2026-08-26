#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vector.h"

enum {
  APPEND_COUNT = 1000000,
  ACCESS_COUNT = 1000000,
  FRONT_INSERT_COUNT = 20000,
};

static uint64_t NanosecondsNow(void) {
  struct timespec timestamp;
  if (clock_gettime(CLOCK_MONOTONIC, &timestamp) != 0) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }
  return (uint64_t)timestamp.tv_sec * UINT64_C(1000000000) +
         (uint64_t)timestamp.tv_nsec;
}

static void Report(const char *label, uint64_t elapsed_nanoseconds,
                   size_t operations) {
  const double elapsed_milliseconds = (double)elapsed_nanoseconds / 1000000.0;
  const double operations_per_second =
      (double)operations * 1000000000.0 / (double)elapsed_nanoseconds;
  (void)printf("%s: %.3f ms, %.0f operations/s\n", label, elapsed_milliseconds,
               operations_per_second);
}

static void RequireSuccess(VectorResult result) {
  if (result != VECTOR_SUCCESS) {
    (void)fprintf(stderr, "vector operation failed: %d\n", result);
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  Vector *vector = CreateVector(sizeof(int));
  volatile uint64_t checksum = 0;
  uint64_t started;

  if (vector == NULL) {
    (void)fputs("could not create vector\n", stderr);
    return EXIT_FAILURE;
  }

  started = NanosecondsNow();
  for (int value = 0; value < APPEND_COUNT; ++value) {
    RequireSuccess(PushBackVector(vector, &value));
  }
  Report("append 1,000,000 ints", NanosecondsNow() - started, APPEND_COUNT);

  started = NanosecondsNow();
  for (size_t index = 0; index < ACCESS_COUNT; ++index) {
    const int *value =
        GetConstVectorElement(vector, index % GetVectorSize(vector));
    checksum += (uint64_t)*value;
  }
  Report("indexed access 1,000,000 ints", NanosecondsNow() - started,
         ACCESS_COUNT);

  DestroyVector(vector);
  vector = CreateVector(sizeof(int));
  if (vector == NULL) {
    (void)fputs("could not create vector\n", stderr);
    return EXIT_FAILURE;
  }
  started = NanosecondsNow();
  for (int value = 0; value < FRONT_INSERT_COUNT; ++value) {
    RequireSuccess(PushFrontVector(vector, &value));
  }
  Report("front insert 20,000 ints", NanosecondsNow() - started,
         FRONT_INSERT_COUNT);

  printf("checksum: %" PRIu64 "\n", checksum);
  DestroyVector(vector);
  return EXIT_SUCCESS;
}
