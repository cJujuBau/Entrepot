# Define the default compiler and flags
CXX ?= g++
CXXFLAGS = -Wall -std=c++11 -Iinclude -Ilib/WiringPi/include

# Define the source files and object files
SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib/WiringPi/lib

SOURCES = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/utils/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Define the target executable
TARGET = $(BINDIR)/robotController.exe

# Define the libraries to link against
LIBS = -L$(LIBDIR) -lwiringPi

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "Using compiler: $(CXX)"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule to build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/utils
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets
.PHONY: all clean