CC := clang
AR ?= ar

CPPFLAGS := -Iinclude -Itests
CSTD := -std=c23
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wformat=2 -Wnull-dereference -Wdouble-promotion -Werror
CFLAGS := $(CSTD) $(WARNINGS) -g

BUILD_DIR := build
LIB_SRC := src/vector.c
TEST_SRC := tests/vector_test.c tests/main.c
LIB_OBJ := $(LIB_SRC:%.c=$(BUILD_DIR)/%.o)
TEST_OBJ := $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)
LIB_STATIC := $(BUILD_DIR)/libvector.a
TEST_BIN := $(BUILD_DIR)/test_vector
FORMAT_FILES := $(shell git ls-files '*.c' '*.h')

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
SDKROOT := $(shell xcrun --show-sdk-path)
export SDKROOT
endif

.PHONY: all build build_lib test lint format format-check clean

all: test $(LIB_STATIC)

build: $(TEST_BIN) $(LIB_STATIC)

build_lib: $(LIB_STATIC)

$(LIB_STATIC): $(LIB_OBJ)
	$(AR) rcs $@ $^

$(TEST_BIN): $(LIB_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule: build/<path>.o from <path>.c
#    e.g. build/src/vector.o ← src/vector.c
#         build/tests/vector.o ← tests/vector.c
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@echo "Running test suite…"
	@./$(TEST_BIN)

# Compile all library and test sources under the warning policy without relying on stale objects.
lint:
	$(CC) $(CPPFLAGS) $(CSTD) $(WARNINGS) -fsyntax-only $(LIB_SRC) $(TEST_SRC)

format:
	clang-format -i $(FORMAT_FILES)

format-check:
	clang-format --dry-run --Werror $(FORMAT_FILES)

clean:
	rm -rf $(BUILD_DIR)
