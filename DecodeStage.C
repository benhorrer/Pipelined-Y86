#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "Stage.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "MemoryStage.h"
#include "DecodeStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "ExecuteStage.h"

bool DecodeStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   E * ereg = (E *) pregs[EREG];
   D * dreg = (D *) pregs[DREG];
   M * mreg = (M *) pregs[MREG];
   W * wreg = (W *) pregs[WREG];
   uint64_t valA = 0, valB = 0;
   uint64_t dstE = RNONE, dstM = RNONE;
   RegisterFile * regInst = RegisterFile::getInstance();
   ExecuteStage * eStage = (ExecuteStage *) stages[ESTAGE];
   MemoryStage * mStage = (MemoryStage *) stages[MSTAGE];



   uint64_t dregIcode = dreg->geticode()->getOutput();
   // set srcA
   d_srcA = setsrcA(dregIcode, dreg);
   //set srcB
   d_srcB = setsrcB(dregIcode, dreg);
   // set dstE
   dstE = setdstE(dregIcode, dreg);
   // set dstM
   dstM = setdstM(dregIcode, dreg);
   // Sel + FwdA 

   valA = selFwdA(d_srcA, eStage->gete_dstE(), eStage->gete_valE(),
             mreg, wreg, dreg, mStage);
   //FwdB
   valB = FwdB(d_srcB, eStage->gete_dstE(), eStage->gete_valE(),
             mreg, wreg, dreg, mStage);
   

   setEInput(ereg, dreg->getstat()->getOutput(), dregIcode,
               dreg->getifun()->getOutput(), dreg->getvalC()->getOutput(),
               valA, valB, dstE, dstM, d_srcA, d_srcB);
    

}

void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t valC, uint64_t valA,
                           uint64_t valB, uint64_t dstE, uint64_t dstM,
                           uint64_t srcA, uint64_t srcB)
{
   ereg->getstat()->setInput(stat);
   ereg->geticode()->setInput(icode);
   ereg->getifun()->setInput(ifun);
   ereg->getvalC()->setInput(valC);
   ereg->getvalA()->setInput(valA);
   ereg->getvalB()->setInput(valB);
   ereg->getdstE()->setInput(dstE);
   ereg->getdstM()->setInput(dstM);
   ereg->getsrcA()->setInput(srcA);
   ereg->getsrcB()->setInput(srcB);
}

void DecodeStage::doClockHigh(PipeReg ** pregs)
{

   E * ereg = (E *) pregs[EREG];

   ereg->getstat()->normal();
   ereg->geticode()->normal();
   ereg->getifun()->normal();
   ereg->getvalC()->normal();
   ereg->getvalA()->normal();
   ereg->getvalB()->normal();
   ereg->getdstE()->normal();
   ereg->getdstM()->normal();
   ereg->getsrcA()->normal();
   ereg->getsrcB()->normal();
}

uint64_t DecodeStage::setsrcA(uint64_t dregIcode, D * dreg) {
    switch (dregIcode) {
        case IRRMOVQ:
        case IRMMOVQ:
        case IOPQ:
        case IPUSHQ:
            return dreg->getrA()->getOutput();
        case IPOPQ:
        case IRET:
            return 4;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::setsrcB(uint64_t dregIcode, D * dreg) {
    switch (dregIcode) {
        case IOPQ:
        case IRMMOVQ:
        case IMRMOVQ:
            return dreg->getrB()->getOutput();
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return 4;
        default:
            return RNONE;
        
    }
}

uint64_t DecodeStage::setdstE(uint64_t dregIcode, D * dreg) {
    switch (dregIcode) {
        case IRRMOVQ:
        case IIRMOVQ:
        case IOPQ:
            return dreg->getrB()->getOutput();
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return 4;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::setdstM(uint64_t dregIcode, D * dreg) {
    switch (dregIcode) {
        case IMRMOVQ:
        case IPOPQ:
            return dreg->getrA()->getOutput();;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::selFwdA(uint64_t srcA, uint64_t e_dstE, uint64_t e_valE,
    M * mreg, W * wreg, D * dreg, MemoryStage * mStage) {
    RegisterFile * regInst = RegisterFile::getInstance();
    uint64_t dregIcode = dreg->geticode()->getOutput();
    if (dregIcode == ICALL || dregIcode == IJXX) return dreg->getvalP()->getOutput();
    else if (srcA == RNONE) return 0;
    else if (srcA == e_dstE) return e_valE;
    else if (srcA == mreg->getdstM()->getOutput()) return mStage->getm_valM();
    else if (srcA == mreg->getdstE()->getOutput()) return mreg->getvalE()->getOutput();
    else if (srcA == wreg->getdstM()->getOutput()) return wreg->getvalM()->getOutput();
    else if (srcA == wreg->getdstE()->getOutput()) return wreg->getvalE()->getOutput();
    else {
        bool selError = false;
        return regInst->readRegister(srcA, selError);
    }
}

uint64_t DecodeStage::FwdB(uint64_t srcB, uint64_t e_dstE, uint64_t e_valE,
         M * mreg, W * wreg, D * dreg, MemoryStage * mStage) {
    RegisterFile * regInst = RegisterFile::getInstance();
    if (srcB == RNONE) return 0;
    else if (srcB == e_dstE) return e_valE;
    else if (srcB == mreg->getdstM()->getOutput()) return mStage->getm_valM();
    else if (srcB == mreg->getdstE()->getOutput()) return mreg->getvalE()->getOutput();
    else if (srcB == wreg->getdstM()->getOutput()) return wreg->getvalM()->getOutput();
    else if (srcB == wreg->getdstE()->getOutput()) return wreg->getvalE()->getOutput();
    else {
        bool selError = false;
        return regInst->readRegister(srcB, selError);
    }
}

uint64_t DecodeStage::getd_srcA() {
    return d_srcA;
}

uint64_t DecodeStage::getd_srcB() {
    return d_srcB;
}

