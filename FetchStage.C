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
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Memory.h"
#include "Tools.h"
#include "Status.h"


/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool FetchStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];
   E * ereg = (E *) pregs[EREG];
   M * mreg = (M *) pregs[MREG];
   W * wreg = (W *) pregs[WREG];
   MemoryStage * mStage = (MemoryStage *) stages[MSTAGE];
   DecodeStage * dStage = (DecodeStage *) stages[DSTAGE];
   ExecuteStage * eStage = (ExecuteStage *) stages[ESTAGE];

   uint64_t f_pc = 0, icode = 0, ifun = 0, valC = 0, valP = 0;
   uint64_t rA = RNONE, rB = RNONE, stat = SAOK;

   //code missing here to select the value of the PC
   //and fetch the instruction from memory
   //Fetching the instruction will allow the icode, ifun,
   //rA, rB, and valC to be set.
   //The lab assignment describes what methods need to be
   //written.
   f_pc = selectPC(freg, mreg, wreg);
   bool memerror = false;
   uint64_t inst = Memory::getInstance()->getByte(f_pc, memerror);

   
   if (!memerror)
   {   
       icode = inst >> 4;
       ifun = 0xf & inst; 
   }
   else {
        icode = INOP;
        ifun = FNONE;
   }
   //stat = f_stat(icode, memerror);
   if (needRegIds(icode))
   {
       uint64_t registerByte = getRegIds(f_pc);
       if (icode == IRRMOVQ  || icode == IRMMOVQ || icode == IMRMOVQ || icode == IOPQ) {
            rA = (registerByte & 0b11110000) >> 4;
            rB = registerByte & 0b1111;
       }

       else if (icode == IPUSHQ || icode == IPOPQ) {
            rA = (registerByte & 0b11110000) >> 4;
            rB = 0xf;
       }

       else {
            rA = 0xf;
            rB = registerByte & 0b1111;
       }
   }
   if (needValC(icode)) {
        valC = buildValC(f_pc, icode);
   }
   uint64_t fstat = f_stat(icode, memerror);
   //The value passed to setInput below will need to be changed
   valP = PCIncrement(f_pc, needRegIds(icode), needValC(icode));

   //stall check
   calculateControlSignals(ereg->geticode()->getOutput(), ereg->getdstM()->getOutput(),
        dStage->getd_srcA(), dStage->getd_srcB(), eStage->gete_Cnd(),
        dreg->geticode()->getOutput(), mreg->geticode()->getOutput());
   pred_pc = predictPC(icode, valC, valP);
   setDInput(dreg, fstat, icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];
   switch(stallF) {
        case true:
            freg->getpredPC()->stall();
            break;
        default:
            freg->getpredPC()->setInput(pred_pc);
            freg->getpredPC()->normal();
            break;

    }
    if (bubbleD) clockBubbleD(dreg);
    else if (stallD) clockStallD(dreg);
    else clockNormalD(dreg);
   
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(D * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->getstat()->setInput(stat);
   dreg->geticode()->setInput(icode);
   dreg->getifun()->setInput(ifun);
   dreg->getrA()->setInput(rA);
   dreg->getrB()->setInput(rB);
   dreg->getvalC()->setInput(valC);
   dreg->getvalP()->setInput(valP);
}

uint64_t FetchStage::selectPC(F * freg, M * mreg, W * wreg)
{
    if (mreg->geticode()->getOutput() == IJXX && !mreg->getCnd()->getOutput()) return mreg->getvalA()->getOutput();
    else if (wreg->geticode()->getOutput() == IRET) return wreg->getvalM()->getOutput();
    return freg->getpredPC()->getOutput();
}

bool FetchStage::needRegIds(uint64_t f_icode)
{
    if (f_icode == IRRMOVQ || f_icode == IOPQ || f_icode == IPUSHQ ||
        f_icode == IPOPQ || f_icode == IIRMOVQ || f_icode == IRMMOVQ ||
        f_icode == IMRMOVQ)
    {
        return true;
    }
    return false;
}

uint64_t FetchStage::getRegIds(uint64_t f_pc)
{  bool error = false;
   uint64_t ids = Memory::getInstance()->getByte(f_pc + 1, error);
   return ids; 
}

bool FetchStage::needValC(uint64_t f_icode)
{
    if (f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ ||
        f_icode == IJXX || f_icode == ICALL)
    {
        return true;
    }
    return false;
}

uint64_t FetchStage::buildValC(uint64_t f_pc, uint64_t f_icode) {
    bool error = false;
    uint64_t f_valC;
    f_pc++;
    if (needRegIds(f_icode)) {
        f_pc++;
    }
    uint8_t byteArray[8];
    for (int j = 0; j < 8; j++) {
        byteArray[j] = Memory::getInstance()->getByte(f_pc++, error);
    }
    f_valC = Tools::buildLong(byteArray);
    return f_valC;
}

uint64_t FetchStage::PCIncrement(uint64_t f_pc, bool f_needRegIds, bool f_needValC)
{
    uint64_t location = f_pc + 1;
    if (f_needRegIds) location += 1;
    if (f_needValC) location += 8;
    return location; 
}

uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP)
{
    if (f_icode == IJXX || f_icode == ICALL) return f_valC;
    return f_valP;
}
     
bool FetchStage::instr_valid(uint64_t f_icode) {
    switch(f_icode) 
    {
        case INOP:
        case IHALT:
        case IRRMOVQ:
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
        case IOPQ:
        case IJXX:
        case ICALL:
        case IRET:
        case IPUSHQ:
        case IPOPQ:
            return true;
        defualt:
            return false;
    }
}

uint64_t FetchStage::f_stat(uint64_t f_icode, bool memerror) {
    if (memerror) return SADR;
    else if (!instr_valid(f_icode)) return SINS;
    else if (f_icode == IHALT) return SHLT;
    else return SAOK;
}

bool FetchStage::f_stall(uint64_t e_icode, uint64_t e_dstM, uint64_t d_srcA, uint64_t d_srcB, 
    uint64_t d_icode, uint64_t m_icode) {
    bool bool1 = e_icode == IMRMOVQ || e_icode == IPOPQ;
    bool bool2 = e_dstM == d_srcA || e_dstM == d_srcB;
    bool bool3 = (e_icode == IRET || d_icode == IRET || m_icode == IRET);
    bool finalCheck = (bool1 && bool2) || bool3;
    return finalCheck;
    
}

bool FetchStage::d_stall(uint64_t e_icode, uint64_t e_dstM, uint64_t d_srcA, uint64_t d_srcB) {
    if((e_icode == IMRMOVQ || e_icode == IPOPQ)
        && (e_dstM == d_srcA || e_dstM == d_srcB)) return true;
    return false;

}

bool FetchStage::d_bubble(uint64_t e_icode, uint64_t e_cnd, uint64_t d_srcA, uint64_t d_srcB,
    uint64_t d_icode, uint64_t m_icode, uint64_t e_dstm) {
    
    bool bubbleRet1 = (e_icode == IMRMOVQ || e_icode == IPOPQ);
    bool bubbleRet2 = (e_dstm == d_srcA || e_dstm == d_srcB);
    bool bubbleRet3 = (e_icode == IRET || d_icode == IRET || m_icode == IRET);
    bool notReturn = !(bubbleRet1 && bubbleRet2);
    bool finalCheck = bubbleRet3 && notReturn;
    return (e_icode == IJXX && !e_cnd) || finalCheck;
    
        
}

void FetchStage::calculateControlSignals(uint64_t e_icode, uint64_t e_dstM, 
    uint64_t d_srcA, uint64_t d_srcB, uint64_t e_cnd, uint64_t d_icode, uint64_t m_icode) {
    if (f_stall(e_icode, e_dstM, d_srcA, d_srcB, d_icode, m_icode)) stallF = true;
    else stallF = false;
    if (d_stall(e_icode, e_dstM, d_srcA, d_srcB)) stallD = true;
    else stallD = false;
    if (d_bubble(e_icode, e_cnd, d_srcA, d_srcB, d_icode, m_icode, e_dstM)) bubbleD = true;
    else bubbleD = false;
}

void FetchStage::clockBubbleD(D * dreg) {
    dreg->getstat()->bubble(SAOK);
    dreg->geticode()->bubble(INOP);
    dreg->getifun()->bubble();
    dreg->getrA()->bubble(RNONE);
    dreg->getrB()->bubble(RNONE);
    dreg->getvalC()->bubble();
    dreg->getvalP()->bubble();
}

void FetchStage::clockStallD(D * dreg) {
     dreg->getstat()->stall();
    dreg->geticode()->stall();
    dreg->getifun()->stall();
    dreg->getrA()->stall();
    dreg->getrB()->stall();
    dreg->getvalC()->stall();
    dreg->getvalP()->stall();
}

void FetchStage::clockNormalD(D * dreg) {
    dreg->getstat()->normal();
    dreg->geticode()->normal();
    dreg->getifun()->normal();
    dreg->getrA()->normal();
    dreg->getrB()->normal();
    dreg->getvalC()->normal();
    dreg->getvalP()->normal();
}
