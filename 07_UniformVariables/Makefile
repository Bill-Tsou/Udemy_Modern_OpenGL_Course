CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -g -O0 $(shell pkg-config --cflags glfw3 glew)
LDFLAGS  := $(shell pkg-config --libs glfw3 glew) -framework OpenGL

TARGET := UniformVariables
SRCS   := $(TARGET).cpp

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(TARGET).dSYM
