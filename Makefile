CC = g++
CFLAGS = -Wall -c -std=c++11
LDFLAGS = -lstdc++
EXECUTABLE = ieee.exe

# Define source and object file paths
SRC_DIR = src
OBJ_DIR = obj
IEEE2_DIR = $(SRC_DIR)/IEEE2
FIR_DIR = $(SRC_DIR)/FIR
TESTS_DIR = $(SRC_DIR)/TESTS

# List all source files
IEEE2_SRCS = $(wildcard $(IEEE2_DIR)/*.cpp)
FIR_SRCS = $(wildcard $(FIR_DIR)/*.cpp)
TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.cpp)
MAIN_SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Define object file names
IEEE2_OBJS = $(patsubst $(IEEE2_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(IEEE2_SRCS))
FIR_OBJS = $(patsubst $(FIR_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(FIR_SRCS))
TESTS_OBJS = $(patsubst $(TESTS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TESTS_SRCS))
MAIN_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRCS))

# Default target
all: $(EXECUTABLE)

# Link object files into executable
$(EXECUTABLE): $(IEEE2_OBJS) $(POSIT_OBJS) $(FIR_OBJS) $(TESTS_OBJS) $(SCM_OBJS) $(MAIN_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# create OBJ_DIR if it is absent
$(shell mkdir -p $(OBJ_DIR))

# Compile IEEE2 source files
$(OBJ_DIR)/%.o: $(IEEE2_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Compile TESTS source files
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Compile FIR source files
$(OBJ_DIR)/%.o: $(FIR_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

	
# Compile main source file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJ_DIR)/* $(EXECUTABLE)
