CC        := clang
CFLAGS    := -std=c11 -Wall -Wextra -g -Iinclude -Itests

BUILD_DIR := build

LIB_SRC   := src/vector.c
TEST_SRC  := tests/vector_test.c tests/main.c

# Derive object‑and‑binary paths under $(BUILD_DIR)
LIB_OBJ   := $(LIB_SRC:%.c=$(BUILD_DIR)/%.o)
TEST_OBJ  := $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)
ALL_OBJ   := $(LIB_OBJ) $(TEST_OBJ)
LIB_STATIC := $(BUILD_DIR)/libvector.a

TEST_BIN  := $(BUILD_DIR)/test_vector

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
SDKROOT := $(shell xcrun --show-sdk-path)
export SDKROOT
endif

.PHONY: all test clean build_lib

all: test $(LIB_STATIC)

build_lib: $(LIB_STATIC)

$(LIB_STATIC): $(LIB_OBJ)
	$(AR) rcs $@ $^

# Link everything into the test binary (in build/)
$(TEST_BIN): $(ALL_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule: build/<path>.o from <path>.c
#    e.g. build/src/vector.o ← src/vector.c
#         build/tests/vector.o ← tests/vector.c
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@echo "⏳ Running tests…"
	@./$(TEST_BIN) || \
	{ echo "❌ Tests failed!"; exit 1; }

clean:
	rm -rf $(BUILD_DIR)
