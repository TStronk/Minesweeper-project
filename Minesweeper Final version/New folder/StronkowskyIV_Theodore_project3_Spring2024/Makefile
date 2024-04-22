SFML_DIR = C:/Users/tedhu/Downloads/SFML-2.5.1
INCLUDE_PATH = $(SFML_DIR)/include
LIB_PATH = $(SFML_DIR)/lib

# Collect all .cpp files in the src/ directory
SOURCES = $(wildcard src/*.cpp)
# Replace .cpp with .o for object files
OBJECTS = $(SOURCES:src/%.cpp=src/%.o)

# Default target
all: build run

# Build the application
build: $(OBJECTS)
	g++ $(OBJECTS) -o sfml-app.out -L$(LIB_PATH) -lsfml-graphics -lsfml-window -lsfml-system

# Run the application
run:
	./sfml-app.out
  
# Pattern rule for object files
src/%.o: src/%.cpp
	g++ -c $< -I$(INCLUDE_PATH) -Wall -Wextra -o $@

# Clean up
clean:
	rm -f src/*.o
	rm -f sfml-app.out