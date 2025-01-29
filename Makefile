CXX = g++

# different compiler flags for different levels of optimization
CXXFLAGS_DEBUG = -g -Og -Wall -Wextra -Wpedantic -Wshadow -Wconversion -DDEBUG
CXXFLAGS_RELEASE = -O2 -march=native -DNDEBUG -Wall -Wextra -Wpedantic

BUILD_DIR = build

TARGET_DEBUG = $(BUILD_DIR)/rbtree_debug
TARGET_RELEASE = $(BUILD_DIR)/rbtree

SRCS = RbTree_t.cpp

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

all: release

release: | $(BUILD_DIR)
	@echo "Building in release mode..."
	@$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET_RELEASE) $(SRCS)

debug: | $(BUILD_DIR)
	@echo "Building in debug mode..."
	@$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRCS)

clean:
	@echo "Cleaning build directory..."
	@rm -f $(TARGET_DEBUG) $(TARGET_RELEASE)

run: release
	@./$(TARGET_RELEASE)

run-debug: debug
	@./$(TARGET_DEBUG)