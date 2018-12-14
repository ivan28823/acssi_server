#Templete to c++ project
CXX ?= g++

# extensions #
SRC_EXT = cpp

# path #
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# executable # 
BIN_NAME = server

# flags #
COMPILE_FLAGS = -Wall -Wextra -g
INCLUDES = -I include
# Space-separated pkg-config libraries used by this project
LIBS = 

#remote host copy
REMOTE_USER = you_user
REMOTE_PASS = yu_pass
REMOTE_HOST = you_host
REMOTE_DIR	= /you/path

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
	@echo "[LOG] Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "[LOG] Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "[LOG] Deleting directories"
	@$(RM) -rf $(BUILD_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: dirs $(BIN_PATH)/$(BIN_NAME)
	@echo "[LOG] Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

# Creation of the executable
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "[LOG] Linking: $@"
	@$(CXX) $(OBJECTS) $(LIBS) -o $@

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "[LOG] Compiling: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install:	$(BIN_PATH)/$(BIN_NAME)
	@echo "[LOG] Installing on /usr/bin/$(BIN_NAME)"
	@sudo cp $(BIN_PATH)/$(BIN_NAME) /usr/bin/

remote_copy:	
	@echo "[LOG] Copying project to remote host: $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)"
	@sshpass -p "$(REMOTE_PASS)" scp -r * $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)

exe: all
	@echo "[LOG] Running $(BIN_NAME)"
	@./$(BIN_NAME)