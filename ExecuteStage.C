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

    

}

void ExecuteStage::setMInput(M * mreg, uint64_t stat, uint64_t icode,
                            uint64_t Cnd, uint64_t valE, uint64_t valA, 
                            uint64_t dstE, uint64_t dstM) {
   mreg->getstat()->setInput(stat);
   mreg->geticode()->setInput(icode);
   mreg->getifun()->setInput(Cnd);
   mreg->getvalC()->setInput(valE);
   mreg->getvalA()->setInput(valA);
   mreg->getdstE()->setInput(dstE);
   mreg->getdstM()->setInput(dstM);
    }

void ExecuteStage::doClockHigh(PipeReg ** pregs)
{

}

