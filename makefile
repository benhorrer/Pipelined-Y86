CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = lab4.o MemoryTester.o Memory.o Tools.o RegisterFile.o \
RegisterFileTester.o ConditionCodes.o ConditionCodesTester.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab4: $(OBJ)

lab4.o: *.h

MemoryTester.o: MemoryTester.h

Memory.o: Memory.h

Tools.o: Tools.h

RegisterFile.o: RegisterFile.h

RegisterFileTester.o: RegisterFileTester.h

ConditionCodes.o: ConditionCodes.h

ConditionCodesTester.o: ConditionCodesTester.h

clean:
	rm $(OBJ) lab4

run:
	make clean
	make lab4
	./run.sh

