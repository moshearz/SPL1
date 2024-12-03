CXX = g++
CXXFLAGS = -Wall -g -c -Weffc++ -I./include

SRCS = $(wildcard src/*.cpp)

OBJS = $(SRCS:.cpp=.o)

TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)