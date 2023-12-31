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
#include "WritebackStage.h"
#include "Status.h"
#include "Debug.h"


bool WritebackStage::doClockLow(PipeReg ** pregs, Stage ** stages) {
    W * wreg = (W *) pregs[WREG];
    w_stat = wreg->getstat()->getOutput();
    if (w_stat != SAOK) return true; 
    return false;
}

void WritebackStage::doClockHigh(PipeReg ** pregs) {
    W * wreg = (W *) pregs[WREG];
    RegisterFile * regInst = RegisterFile::getInstance();

    bool error = false;
    regInst->writeRegister(wreg->getvalE()->getOutput(), wreg->getdstE()->getOutput(), error);
    regInst->writeRegister(wreg->getvalM()->getOutput(), wreg->getdstM()->getOutput(), error);

}

uint64_t WritebackStage::getw_stat() {
    return w_stat;
}

