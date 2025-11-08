# ----------------------------
# Compiler & flags
# ----------------------------
CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
DEBUG_FLAGS := -Og -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
RELEASE_FLAGS := -O2 -march=native -flto -DNDEBUG

# ----------------------------
# Directories
# ----------------------------
BUILD_DIR := build

BUILD_RELEASE := $(BUILD_DIR)/release
BUILD_DEBUG   := $(BUILD_DIR)/debug

# ----------------------------
# Targets
# ----------------------------
TARGET := ray

# Source files
SRCS := $(wildcard *.c)

# ----------------------------
# Default target
# ----------------------------
all: release

# ----------------------------
# Build main binary
# ----------------------------
release: CFLAGS += $(RELEASE_FLAGS)
release: $(BUILD_RELEASE)/$(TARGET)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BUILD_DEBUG)/$(TARGET)

$(BUILD_RELEASE)/$(TARGET): $(patsubst %.c,$(BUILD_RELEASE)/%.o,$(SRCS))
	@mkdir -p $(BUILD_RELEASE)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DEBUG)/$(TARGET): $(patsubst %.c,$(BUILD_DEBUG)/%.o,$(SRCS))
	@mkdir -p $(BUILD_DEBUG)
	$(CC) $(CFLAGS) -o $@ $^

# ----------------------------
# Compile object files (pattern rules)
# ----------------------------
$(BUILD_RELEASE)/%.o: %.c
	@mkdir -p $(BUILD_RELEASE)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DEBUG)/%.o: %.c
	@mkdir -p $(BUILD_DEBUG)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ----------------------------
# Include dependency files
# ----------------------------
-include $(wildcard $(BUILD_RELEASE)/*.d)
-include $(wildcard $(BUILD_DEBUG)/*.d)

# ----------------------------
# Clean
# ----------------------------
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all release debug clean
