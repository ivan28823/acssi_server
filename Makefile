#Templete to c++ project
CXX ?= g++

# extensions #
SRC_EXT = cpp

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = ascii_server

# flags #
COMPILE_FLAGS = -Wall -Wextra -g
INCLUDES = -I include
# Space-separated pkg-config libraries used by this project
LIBS = 

#remote host copy
REMOTE_USER = ivan
REMOTE_HOST = 192.168.1.10
REMOTE_DIR  = ~/UserFiles/WorkSpace/C_Cpp/AsciiServer/

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "[MAKE] Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "[MAKE] Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "[MAKE] Deleting directories"
	@$(RM) -rf $(BUILD_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "[MAKE] Making symlink: $(BIN_NAME) -> $(BIN_PATH)/$(BIN_NAME)"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "[MAKE] Linking: $@"
	@$(CXX) $(OBJECTS) $(LIBS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "[MAKE] Compiling: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install:	$(BIN_PATH)/$(BIN_NAME)
	@echo "[MAKE] Installing on /usr/bin/$(BIN_NAME)"
	@sudo cp $(BIN_PATH)/$(BIN_NAME) /usr/bin/

remote_copy:	
	@echo "[MAKE] Copying project to remote host: $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)"
	@scp -r * $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)

exe: all
	@echo "[MAKE] Running $(BIN_NAME)"
	@./$(BIN_NAME)
