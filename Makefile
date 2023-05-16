# Copyright (c) 2020 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Set global macros
buildDir := bin
libName := libimnotgui
target := $(buildDir)/$(libName).a
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))
compileFlags := -std=c++17 -I include
linkFlags = -L lib/$(platform) -l raylib

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	AR ?= ar
	MAKE ?= mingw32-make
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -mwindows -static -static-libgcc -static-libstdc++
	libGenDir := src
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := Linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
		libGenDir := src
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOS
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
		libGenDir := src
	endif

	# Set UNIX macros
	AR ?= ar
	MAKE ?= make
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

# Lists phony targets for Makefile
.PHONY: all example clean setup submodules

# Default target, compiles, executes and cleans
all: $(buildDir) $(target) example

# Make build folder
$(buildDir):
	$(MKDIR) $(call platformpth, $(buildDir))

# Build the imnotgui static library file and copy it into bin
$(target): $(objects)
	$(AR) rcs $(target) $(objects)

# Build examples and run
example: $(target)
	$(MAKE) test -C example

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)

# Clean up all relevant files
clean:
	$(RM) $(call platformpth, $(buildDir)/*)

# Add all rules from dependency files
-include $(depends)


############ Resolving Dependencies ############


# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive --remote

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include)
	$(MKDIR) $(call platformpth, ./include/raylib)
	$(call COPY,vendor/raylib/src,./include/raylib,raylib.h)
	$(call COPY,vendor/raylib/src,./include/raylib,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,./include/raylib,*.hpp)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib/$(libGenDir),lib/$(platform),libraylib.a)