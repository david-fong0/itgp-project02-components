CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 $(shell pkg-config --cflags sdl2 yaml-cpp)
LDLIBS := $(shell pkg-config --libs sdl2 yaml-cpp)
SOURCES := main.cpp Engine.cpp Input.cpp Components.cpp ComponentFactory.cpp View.cpp
TARGET := EscapeThroughTime.exe

all: $(TARGET)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDLIBS)
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(TARGET)
.PHONY: all run clean
