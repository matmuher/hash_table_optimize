CC=g++
ASM=nasm
ASMFLAGS=-felf64
CFLAGS=-g -c -O1 -Wall -mavx -mavx2
LDFLAGS=
INCLUDE=Include/libaelf64.a
SOURCES=main.cpp hash_table.cpp input_proc.cpp stress_test.cpp
ASM_SOURCES=A_pjw_hash.asm A_rol_hash.asm
OBJECTS=$(SOURCES:.cpp=.o)
ASM_OBJECTS=$(ASM_SOURCES:.asm=.o)
EXECUTABLE=hash_table

all: $(SOURCES) $(ASM_SOURCES) $(EXECUTABLE)
	./${EXECUTABLE}
	
$(EXECUTABLE): $(ASM_OBJECTS) $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(ASM_OBJECTS) $(INCLUDE) -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 

leak_check: $(SOURCES) ${EXECUTABLE}
	valgrind --leak-check=yes ./${EXECUTABLE}

stress_test: $(SOURCES) ${EXECUTABLE}
	valgrind --tool=callgrind ./${EXECUTABLE}

clean:
	rm -rf *.o
	ls | grep -v "\.\|Makefile" | xargs rm
