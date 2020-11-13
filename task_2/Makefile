CC := g++ -std=c++2a -O3 -pthread
CFLAGS := -c -Wall
SOURCES =  main.cpp solution.cpp mutation.cpp tinyxml2.cpp
GEN_LIST = tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp genxml.cpp
OBJECTS = $(SOURCES:.cpp=.o)
GEN_OB = $(GEN_LIST:.cpp=.o)

ALG := simulated
GEN := generate


all: $(ALG) $(GEN)

simulated:: $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@

generate: $(GEN_OB)
	$(CC) $(GEN_OB) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf  *.o
