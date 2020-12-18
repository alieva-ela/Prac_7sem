CC := g++-9 -O3 -fopenmp
CFLAGS := -c -Wall
SOURCES =  ga.cpp
OBJECTS = $(SOURCES:.cpp=.o)

ALG := ga

ga:: $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o
