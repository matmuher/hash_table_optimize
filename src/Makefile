CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp hash_table.cpp input_proc.cpp stress_test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hash_table

all: $(SOURCES) $(EXECUTABLE)
	./${EXECUTABLE}
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

leak_check: $(SOURCES) ${EXECUTABLE}
	valgrind --leak-check=yes ./${EXECUTABLE}

clean:
	rm -rf *.o
	ls | grep -v "\.\|Makefile" | xargs rm
