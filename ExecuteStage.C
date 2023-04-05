#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"


bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    E * ereg = (E *) pregs[EREG];
    M * mreg = (M *) pregs[MREG];

    uint64_t Cnd = 0, valE = 0;

    uint64_t eregIcode = ereg->geticode()->getOutput();
    
    setMInput(mreg, ereg->getstat()->getOutput(), eregIcode,
                ereg->getCnd()->getOutput(), ereg->getdstE()->getOutput(),
                ereg->getdstM()->getOutput(), ereg->getvalA()->getOutput(),
                Cnd, valE);

}

void ExecuteStage::setMInput(M * mreg, uint64_t stat, uint64_t icode,
                            uint64_t dstE, uint64_t dstM, uint64_t valA, 
                            uint64_t Cnd, uint64_t valE) 
{
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getCnd()->setInput(Cnd);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}

void ExecuteStage::doClockHigh(PipeReg ** pregs)
{

    M * mreg = (E *) pregs[MREG];

    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getCnd()->setInput(Cnd);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}

