# c-vector

`c-vector` is a small generic, contiguous vector library for C. It stores
fixed-size elements by copying their bytes, supports custom allocators, and
provides indexed access plus insertion and removal at either end.

## Usage

Include `vector.h`, create a vector with the size of its element type, and pass
pointers to values when adding elements:

```c
#include "vector.h"

Vector *numbers = CreateVector(sizeof(int));
int value = 42;

if (numbers != NULL && PushBackVector(numbers, &value) == VECTOR_SUCCESS) {
  const int *first = GetConstFrontVector(numbers);
  /* use *first */
}

DestroyVector(numbers);
```

Vectors own their storage but do not manage resources held by their elements.
A reallocation invalidates every pointer returned by an accessor; insertion,
removal, and shrinking can invalidate or change affected element pointers.
Insert and append operations copy values; an inserted value may safely be an
element already in the same vector.

## Requirements and development

The project uses C23, `make`, and a Clang toolchain. [mise](https://mise.jdx.dev/)
installs the pinned Clang, `clang-format`, and `clang-tidy` versions.

```console
$ mise install
$ mise run test          # Build and run the test suite
$ mise run lint          # Compile all targets with warnings as errors
$ mise run tidy          # Run the SEI CERT C static-analysis policy
$ mise run format-check  # Check formatting
$ mise run check         # Run the required quality gate
$ mise run benchmark     # Run vector operation benchmarks
```

To build directly:

```console
$ make build
$ ./build/test_vector
```

## API documentation

Public APIs in [`include/vector.h`](include/vector.h) use Doxygen comments.
With [Doxygen](https://www.doxygen.nl/) installed, generate the API reference
with:

```console
$ make docs
```

The generated site is written to `build/docs/html/index.html`.

## Behaviour and performance

The vector begins with capacity for 10 elements and grows geometrically as
needed. Indexed access, size queries, and capacity queries are constant time.
Appending is amortized constant time; insertion or removal away from the end is
linear in the number of shifted elements. `ReserveVector` can be used to avoid
growth reallocations when the required capacity is known in advance.

Most mutating functions return `VectorResult` rather than aborting. Creation
returns `NULL` on invalid input, overflow, or allocation failure. See the
generated API reference for the full result and argument contract.
