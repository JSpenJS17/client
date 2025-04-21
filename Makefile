# Client Makefile

ifeq ($(OS), Windows_NT)
	OS_TYPE := Windows
else
	UNAME_S := $(shell uname -s)

	ifeq ($(UNAME_S), Linux)
		OS_TYPE := Linux
	else ifeq ($(UNAME_S), Darwin)
		OS_TYPE := MacOS
	else
		OS_TYPE := Unknown
	endif
endif

$(info Detected OS: $(OS_TYPE))

GXX := g++
CPP := cpp
HPP := hpp

TARGET := client
SRC_DIR := src
OBJ_DIR := $(SRC_DIR)/obj
SRC := $(wildcard $(SRC_DIR)/*.$(CPP))
OBJ := $(patsubst $(SRC_DIR)/%.$(CPP), $(OBJ_DIR)/%.o, $(SRC))
HDR := $(wildcard $(SRC_DIR)/*.$(HPP))

ifeq ($(OS_TYPE), Windows)
	MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
# only need socket libraries on windows
	CFLAGS := -Wall -lws2_32
else
	MKDIR = mkdir -p $(1)
	CFLAGS := -Wall
endif

# Final executable target
$(TARGET): $(HDR) $(OBJ)
	$(GXX) $(OBJ) -o $(TARGET)

# Rule to compile .cpp -> .o (ensures obj/ exists)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(CPP) | $(OBJ_DIR)
	$(GXX) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files (generated automatically)
-include $(OBJ_DIR)/*.d

# Create the obj/ directory if it doesn't exist
$(OBJ_DIR):
	$(call MKDIR, $(OBJ_DIR))

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
