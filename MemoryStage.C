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
#include "MemoryStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Memory.h"


bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    M * mreg = (M *) pregs[MREG];
    W * wreg = (W *) pregs[WREG];
    Memory * mem = Memory::getInstance();
    uint64_t stat;
    uint64_t icode;
    uint64_t valE;
    uint64_t valM;
    uint64_t dstE;
    uint64_t dstM;
    uint64_t addr;
    bool error = false;
    RegisterFile * regInst = RegisterFile::getInstance();

    stat = mreg->getstat()->getOutput();
    icode = mreg->geticode()->getOutput();
    valE = mreg->getvalE()->getOutput();
    valM = 0;
    dstE = mreg->getdstE()->getOutput();
    dstM = mreg->getdstM()->getOutput();
    addr = memAddr(icode, mreg);

    if(icode == IPUSHQ) {
        valE = regInst->readRegister(RSP, error) - 8;
    }

    if (mem_read(icode)) {
        valM = mem->getLong(addr, error);
    }

    if (mem_write(icode)) {
        mem->putLong(mreg->getvalA()->getOutput(), addr, error);
    }

    


    setWInput(wreg, stat, icode, valE, valM, dstE, dstM);

    
}

void MemoryStage::doClockHigh(PipeReg ** pregs)
{   
    W * wreg = (W *) pregs[WREG];


    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}

void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE,
                uint64_t valM, uint64_t dstE, uint64_t dstM) 
{
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(valM);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);

}

uint64_t MemoryStage::memAddr(uint64_t m_icode, M * _mreg) {

    if (m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL || m_icode == IMRMOVQ) {
        return _mreg->getvalE()->getOutput();
    }
    else if (m_icode == IPOPQ || m_icode == IRET) {
        return _mreg->getvalA()->getOutput();
    }
    return 0;
}

bool MemoryStage::mem_read(uint64_t m_icode) {
    if (m_icode == IMRMOVQ || m_icode == IPOPQ || m_icode == IRET) return true;
    return false;
}

bool MemoryStage::mem_write(uint64_t m_icode) {
    if (m_icode == IRMMOVQ || m_icode == IPUSHQ || m_icode == ICALL) return true;
    return false;
}
