CC = g++
CFLAGS = -g -c -Wall -std=c++11 -Og
OBJ = yess.o ConditionCodes.o Memory.o Tools.o RegisterFile.o \
Loader.o PipeReg.o Simulate.o PipeRegField.o FetchStage.o F.o DecodeStage.o D.o \
ExecuteStage.o E.o MemoryStage.o M.o WritebackStage.o W.o
HEADEROBJ = ConditionCodes.h Memory.h Tools.h RegisterFile.h \
Loader.h Debug.h PipeReg.h Stage.h Simulate.h 

.C.o:
	$(CC) $(CFLAGS) $< -o $@

yess: $(OBJ)

yess.o: $(HEADEROBJ)

Loader.o: Memory.o Loader.o

Memory.o: Tools.h Memory.h

Tools.o: Tools.h

RegisterFile.o: RegisterFile.h

FetchStage.o: RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h \
Stage.h FetchStage.h Status.h Debug.h

D.o: Instructions.h RegisterFile.h PipeReg.h PipeRegField.h Status.h

E.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

F.o: PipeRegField.h PipeReg.h

FetchStage.o: RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h \
Stage.h Status.h Debug.h

M.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

PipeReg.o: 

PipeRegField.o:

Simulate.o: PipeRegField.h PipeReg.h F.h D.h E.h M.h W.h \
Stage.h ExecuteStage.h MemoryStage.h DecodeStage.h FetchStage.h WritebackStage.h \
Memory.h RegisterFile.h ConditionCodes.h

W.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h Status.h

ConditionCodes.o: ConditionCodes.h Tools.h

clean:
	rm $(OBJ)

run:
	make clean
	make yess
	./run.sh
