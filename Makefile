CC = g++
CFLAGS = --std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
# Your .hpp files
DEPS = Universe.hpp CelestialBody.hpp 
# Your compiled .o files
OBJECTS = Universe.o CelestialBody.o
# The name of your program
PROGRAM = NBody 

.PHONY: all clean lint

all: $(PROGRAM) test NBody.a

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

NBody.a: Universe.o CelestialBody.o
	ar rcs NBody.a Universe.o CelestialBody.o

clean:
	rm *.o $(PROGRAM) test NBody.a

lint:
	cpplint *.cpp *.hpp
