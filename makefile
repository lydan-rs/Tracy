COMPILER = clang
CFLAGS = -g -Wall
EXECUTABLE = tracy
BUILD_DIR = build
TARGET = $(BUILD_DIR)/$(EXECUTABLE)

SRC = ./src/main.c

# What is .PHONY??
.PHONY: build run clean


$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	@echo "Building Tracy"
	$(COMPILER) $(CFLAGS) -o $(TARGET) ./src/main.c
	@echo "Build Complete"

build: $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
