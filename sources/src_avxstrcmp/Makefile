CC=g++
CFLAGS=-g -c -O0 -Wall -mavx -mavx2
LDFLAGS=
INCLUDE=Include/libaelf64.a
SOURCES=main.cpp hash_table.cpp input_proc.cpp stress_test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hash_table

all: $(SOURCES) $(EXECUTABLE)
	./${EXECUTABLE}
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(INCLUDE) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

leak_check: $(SOURCES) ${EXECUTABLE}
	valgrind --leak-check=yes ./${EXECUTABLE}

stress_test: $(SOURCES) ${EXECUTABLE}
	valgrind --tool=callgrind ./${EXECUTABLE}

clean:
	rm -rf *.o
	ls | grep -v "\.\|Makefile" | xargs rm
