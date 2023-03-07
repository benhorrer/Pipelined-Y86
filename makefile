CC = g++
CFLAGS = -g -c -Wall -std=c++11 -Og
OBJ = lab5.o ConditionCodes.o Memory.o Tools.o RegisterFile.o \
Loader.o 

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab5: $(OBJ)

Loader.o: Memory.o Loader.o

Memory.o: Tools.h Memory.h

Tools.o: Tools.h

RegisterFile.o: RegisterFile.h

lab5.o: *.h

ConditionCodes.o: ConditionCodes.h Tools.h

clean:
	rm $(OBJ)

run:
	make clean
	make lab5
	./run.sh
