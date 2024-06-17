# Define the compiler
CXX = g++
# Compiler flags
CXXFLAGS = -Wall -std=c++17 -pthread
# Output executable name
TARGET = embedded_parking_assist
# Source files
SRCS = main.cpp Simulator.cpp Sensor.cpp Obstacle.cpp Cpu.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)
# Header files
HEADERS = Ping.h 

# Default rule to build the executable
all: $(TARGET)

# Rule to link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile the source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
