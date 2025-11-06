TARGET = ray
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Wshadow -Wconversion 
DEBUG_FLAGS = -Og -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
RELEASE_FLAGS = -O2 -march=native -flto -DNDEBUG

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: release

release: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^

debug: $(TARGET)_debug

$(TARGET)_debug: $(SRCS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^

clean:
	rm -rf $(TARGET) $(TARGET)_debug $(OBJS) *.dSYM

.PHONY: all release debug clean
