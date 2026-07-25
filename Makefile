# ─── APC – Arbitrary Precision Calculator ─────────────────────────────────
CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11 -g -Iinclude
LDFLAGS =

SRC_DIR   = src
BUILD_DIR = build
TARGET    = apc

SRCS = $(SRC_DIR)/apc.c $(SRC_DIR)/main.c
OBJS = $(BUILD_DIR)/apc.o $(BUILD_DIR)/main.o

# ─── Default target ────────────────────────────────────────────────────────
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build successful → ./$(TARGET)"

$(BUILD_DIR)/apc.o: $(SRC_DIR)/apc.c include/apc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c include/apc.h
	$(CC) $(CFLAGS) -c $< -o $@

# ─── Run ───────────────────────────────────────────────────────────────────
run: all
	./$(TARGET)

# ─── Test (quick smoke-test) ───────────────────────────────────────────────
test: all
	@echo "Running smoke tests..."
	@echo "123 + 456"          | ./$(TARGET) | grep -q "579"       && echo "  ✓ Addition"
	@echo "1000 - 1"           | ./$(TARGET) | grep -q "999"       && echo "  ✓ Subtraction"
	@echo "99999 * 99999"      | ./$(TARGET) | grep -q "9999800001" && echo "  ✓ Multiplication"
	@echo "100 / 7"            | ./$(TARGET) | grep -q "14"        && echo "  ✓ Division"
	@echo "-50 + 30"           | ./$(TARGET) | grep -q "\-20"      && echo "  ✓ Negative numbers"
	@echo "All tests passed!"

# ─── Clean ─────────────────────────────────────────────────────────────────
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run test clean
