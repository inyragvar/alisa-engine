########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++11 -ggdb -Wall

# Makefile settings - Can be customized.
APPNAME = test_game
TESTAPP = test_game_tests
EXT := .cc .c
SRC := ../../src
TEST_SRC := ../../tests
LIB := ../../../libs/bin/
INC := ../../src
TEST_INC := ../../tests

BUILD := build
BUILD_TEST := tests

# Library search directories and flags
EXT_LIB :=
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLEW -lassimp
LDPATHS := $(addprefix -L,$(LIB) $(EXT_LIB))

# Include directories
INC_DIRS := $(INC)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

TEST_INC_DIRS := $(TEST_INC)
TEST_INC_FLAGS := $(addprefix -I,$(TEST_INC_DIRS))

LDFLAGS_TEST := -lgtest -lgtest_main -lpthread

# Find both .cc and .c files in the source directories
SRCS := $(foreach ext,$(EXT),$(shell find $(SRC) -name *$(ext)))
TEST_SRCS := $(foreach ext,$(EXT),$(shell find $(TEST_SRC) -name *$(ext)))

OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))
DEPS := $(OBJS:.o=.d)

TEST_OBJS := $(subst $(TEST_SRC)/,$(BUILD_TEST)/,$(addsuffix .o,$(basename $(TEST_SRCS))))
TEST_DEPS := $(TEST_OBJS:.o=.d)

OBJS_NO_MAIN := $(filter-out $(BUILD)/main.o, $(OBJS))

# Run task
run: $(APPNAME)
	@echo "🚀 Executing..."
	./$(APPNAME) $(ARGS)

# Build task
.PHONY: build
build: $(APPNAME)

# Main task
$(APPNAME): $(OBJS)
	@echo "🚧 Building..."
	mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDPATHS)

# Compile all .cc files
$(BUILD)/%.o: $(SRC)/%.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c -o $@ $<

# Compile all .c files
$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	gcc -std=c11 -ggdb -Wall $(INC_FLAGS) -c -o $@ $<

# Task to tests
.PHONY: tests
tests: $(TESTAPP)
	@echo "🚀 Running Tests..."
	./$(TESTAPP)

# Main unitest task
$(TESTAPP): $(TEST_OBJS)
	@echo "🚧 Building..."
	mkdir -p $(dir $@)
	$(CXX) $(TEST_OBJS) $(OBJS_NO_MAIN) -o $@ $(LDFLAGS) $(LDPATHS) $(LDFLAGS_TEST)

vpath %.cc $(TEST_SRC)

# Compile all unitest cc files
$(BUILD_TEST)/%.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $(TEST_INC_FLAGS) -c -o $@ $<

# Clean task
.PHONY: clear
clear:
	@echo "🧹 Clearing..."
	rm -rf $(BUILD)
	rm -rf $(BUILD_TEST)

# Clean task
.PHONY: clear-tests
clear-tests:
	@echo "🧹 Clearing..."
	rm -rf $(BUILD_TEST)

# Include all dependencies
-include $(DEPS)
-include $(TEST_DEPS)
