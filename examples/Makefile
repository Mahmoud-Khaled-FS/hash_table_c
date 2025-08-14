CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INCLUDE = -I..

BUILD_DIR = build

ifeq ($(OS),Windows_NT)
		SHELL := powershell.exe
    EXAMPLES = $(BUILD_DIR)/basic_example.exe $(BUILD_DIR)/number_example.exe $(BUILD_DIR)/performance_test.exe
    MKDIR = if (-not (Test-Path "$(BUILD_DIR)")) { mkdir $(BUILD_DIR) }
		RM = Remove-Item -Path "$(BUILD_DIR)\*.exe" 
else
    EXAMPLES = $(BUILD_DIR)/basic_example $(BUILD_DIR)/number_example $(BUILD_DIR)/performance_test
    MKDIR = mkdir -p $(BUILD_DIR)
		RM = rm -f "./build/*"
endif

all: $(EXAMPLES)



$(BUILD_DIR)/%: %.c
	$(MKDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.exe: %.c
	$(MKDIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM)


run: all
	@echo "=== Run Basic Example ==="
	$(BUILD_DIR)/basic_example
	@echo "=== Run Number Example ==="
	$(BUILD_DIR)/number_example
	@echo "=== Run Performance Test ==="
	$(BUILD_DIR)/performance_test

.PHONY: all clean run