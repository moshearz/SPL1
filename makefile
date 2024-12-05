CXX = g++
CXXFLAGS = -Wall -g -o0 -c -Weffc++ -I./include

SRCS = $(wildcard src/*.cpp)

OBJ_DIR = bin
OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: src/%.cpp
	mkdir -p $(OBJ_DIR) # Ensure the bin/ directory exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
