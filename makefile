CC = g++
CFLAGS = -g -c -Wall -std=c++11 -Og
OBJ = lab4.o MemoryTester.o Memory.o Tools.o RegisterFile.o \
RegisterFileTester.o ConditionCodes.o ConditionCodesTester.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab4: $(OBJ)

Memory.o: Tools.h Memory.h

Tools.o: Tools.h

MemoryTester.o: MemoryTester.h Memory.h

RegisterFile.o: RegisterFile.h

RegisterFileTester.o: RegisterFile.h RegisterFileTester.h

lab4.o: *.h

ConditionCodes.o: ConditionCodes.h Tools.h

ConditionCodesTester.o: ConditionCodes.h ConditionCodesTester.h

clean:
	rm $(OBJ)

run:
	make clean
	make lab4
	./run.sh

