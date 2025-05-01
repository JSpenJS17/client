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
LIB_DIR := $(SRC_DIR)/lib
OBJ_DIR := $(SRC_DIR)/obj
LOB_DIR := $(SRC_DIR)/obj/lib

SRC := $(wildcard $(SRC_DIR)/*.$(CPP))
SRC += $(wildcard $(LIB_DIR)/*.$(CPP))

HDR := $(wildcard $(SRC_DIR)/*.$(HPP))
HDR += $(wildcard $(LIB_DIR)/*.$(HPP))

OBJ := $(patsubst $(SRC_DIR)/%.$(CPP), $(OBJ_DIR)/%.o, $(SRC))
OBJ := $(patsubst $(SRC_DIR)/%.$(CPP), $(OBJ_DIR)/%.o, $(SRC))

$(info $(OBJ))

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
	$(GXX) $(OBJ) $(CFLAGS) -o $(TARGET)

# Rule to compile .cpp -> .o (ensures obj/ exists)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(CPP) | $(OBJ_DIR) $(LOB_DIR)
	$(GXX) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files (generated automatically)
-include $(OBJ_DIR)/*.d
-include $(LOB_DIR)/*.d

# Create the obj/ directory if it doesn't exist
$(OBJ_DIR):
	$(call MKDIR, $(OBJ_DIR))

$(LOB_DIR):
	$(call MKDIR, $(LOB_DIR))

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(LOB_DIR)/*.o $(OBJ_DIR)/*.d $(LOB_DIR)/*.d $(TARGET)
