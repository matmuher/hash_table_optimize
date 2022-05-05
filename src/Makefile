CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp hash_functions.cpp input_proc.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hash_table

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

leak_check: hash_table
	valgrind --leak-check=yes ./hash_table

clean:
	rm -rf *.o
	ls | grep -v "\.\|Makefile" | xargs rm