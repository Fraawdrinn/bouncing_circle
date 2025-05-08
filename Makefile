# Compiler
CXX = g++

# Target executable
TARGET = bouncing_circles

# Source files
SRCS = main.cpp

# Libraries
LIBS = -lglfw -lGLEW -lGL

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
    $(CXX) $(SRCS) -o $(TARGET) $(LIBS)

# Clean up build files
clean:
    rm -f $(TARGET)