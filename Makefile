CXX = g++

CXXFLAGS_DEBUG = -g -Og -Wall -Wextra -Wpedantic -Wshadow -Wconversion -DDEBUG
CXXFLAGS_RELEASE = -O2 -march=native -DNDEBUG -Wall -Wextra -Wpedantic

BUILD_DIR = build

TARGET_DEBUG = $(BUILD_DIR)/rbtree_debug
TARGET_RELEASE = $(BUILD_DIR)/rbtree

SRCS = RbTree_t.cpp

all: release

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

release: | $(BUILD_DIR)
	@echo "building in release mode..."
	@$(CXX) $(CXXFLAGS_RELEASE) -o $(TARGET_RELEASE) $(SRCS)

debug: | $(BUILD_DIR)
	@echo "building in debug mode..."
	@$(CXX) $(CXXFLAGS_DEBUG) -o $(TARGET_DEBUG) $(SRCS)

clean:
	@echo "cleaning build directory..."
	@rm -f $(TARGET_DEBUG) $(TARGET_RELEASE)

run: | release
	@echo "run in release mode..."
	@./$(TARGET_RELEASE)

run-debug: | debug
	@echo "run in debug mode..."
	@./$(TARGET_DEBUG)