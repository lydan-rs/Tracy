COMPILER = clang
CFLAGS = -g -Wall
EXECUTABLE = tracy

SOURCE_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/objects

TARGET = $(BUILD_DIR)/$(EXECUTABLE)

# Relative to the ./src directory
SOURCES = main.c linalg/vec3.c image/image.c
# SRC_FILES = $(addprefix $(SOURCE_DIR)/,$(SOURCES))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

# What is .PHONY??
.PHONY: build run clean

%.o: $(SOURCE_DIR)/%.c
	@echo "Compiling $^"
	$(COMPILER) $(CFLAGS) -c -o $@ $^
	@echo "$^ successfully compiled"

$(TARGET): $(OBJECTS)
	@mkdir -p $(SOURCE_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	@echo "SOURCES = $(SOURCES)"
	@echo "Objects = $(OBJECTS)"
	@echo "Building Tracy"
	$(COMPILER) $(CFLAGS) -o $@ $^
	@echo "Build Complete"



# $(TARGET): $(SOURCES)
# 	# @mkdir -p $(BUILD_DIR)
# 	@echo "Building Tracy"
# 	$(COMPILER) $(CFLAGS) -o $(TARGET) ./src/$(SOURCES)
# 	@echo "Build Complete"

build: $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
