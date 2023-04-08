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


bool WritebackStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    W * wreg = (W *) pregs[WREG];
    uint64_t icode = wreg->geticode()->getOutput();
    if (icode == 0x00) return true; 
    return false;
}

void WritebackStage::doClockHigh(PipeReg ** pregs)
{

}

