#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Memory.h"

bool MemoryStage::doClockLow(PipeReg **pregs, Stage **stages)
{
    M *mreg = (M *)pregs[MREG];
    W *wreg = (W *)pregs[WREG];
    E *ereg = (E *)pregs[EREG];
    Memory *mem = Memory::getInstance();
    
    uint64_t valE;
    uint64_t dstE;
    uint64_t dstM;
    uint64_t addr;
    mem_error = false;
    RegisterFile *regInst = RegisterFile::getInstance();

    m_icode = mreg->geticode()->getOutput();
    m_ifun = ereg->getifun()->getOutput();
    valE = mreg->getvalE()->getOutput();
    valM = 0;
    dstE = mreg->getdstE()->getOutput();
    dstM = mreg->getdstM()->getOutput();
    addr = memAddr(m_icode, mreg);
    if (mem_read(m_icode))
    {
        valM = mem->getLong(addr, mem_error);
    }

    if (mem_write(m_icode))
    {
        mem->putLong(mreg->getvalA()->getOutput(), addr, mem_error);
    }
    m_stat = setMStat(mem_error, mreg);


    setWInput(wreg, m_stat, m_icode, valE, valM, dstE, dstM);
}

void MemoryStage::doClockHigh(PipeReg **pregs)
{
    W *wreg = (W *)pregs[WREG];

    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}

void MemoryStage::setWInput(W *wreg, uint64_t stat, uint64_t icode, uint64_t valE,
                            uint64_t valM, uint64_t dstE, uint64_t dstM)
{
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(valM);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}

uint64_t MemoryStage::memAddr(uint64_t m_icode, M *_mreg)
{

    switch (m_icode)
    {
    case IRMMOVQ:
    case IPUSHQ:
    case ICALL:
    case IMRMOVQ:
        return _mreg->getvalE()->getOutput();
    case IPOPQ:
    case IRET:
        return _mreg->getvalA()->getOutput();
    default:
        return 0;
    }
}

bool MemoryStage::mem_read(uint64_t m_icode)
{
    switch (m_icode)
    {
    case IMRMOVQ:
    case IPOPQ:
    case IRET:
        return true;
    default:
        return false;
    }
}

bool MemoryStage::mem_write(uint64_t m_icode)
{
    switch (m_icode)
    {
    case IRMMOVQ:
    case IPUSHQ:
    case ICALL:
        return true;
    default:
        return false;
    }
}

uint64_t MemoryStage::setMStat(bool memerror, M *mreg) {
    if (memerror) {
        return SADR;
    }
    return mreg->getstat()->getOutput();
}

uint64_t MemoryStage::getm_valM()
{
    return valM;
}

bool MemoryStage::getMem_error() 
{
    return mem_error;
}

uint64_t MemoryStage::getm_stat() {
    return m_stat;
}

uint64_t MemoryStage::getm_icode() {
    return m_icode;
}

uint64_t MemoryStage::getm_ifun() {
    return m_ifun;
}