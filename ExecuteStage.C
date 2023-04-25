#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "E.h"
#include "Stage.h"
#include "ExecuteStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Tools.h"
#include "ConditionCodes.h"

bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    E * ereg = (E *) pregs[EREG];
    M * mreg = (M *) pregs[MREG];
    D * dreg = (D *) pregs[DREG];

    uint64_t Cnd = 0;
    bool error = false;
    uint64_t e_cnd;
    uint64_t eregIcode = ereg->geticode()->getOutput();
    RegisterFile * regInst = RegisterFile::getInstance();
    e_valE = ereg->getvalC()->getOutput();
    uint64_t aluA = 0;
    uint64_t aluB = 0;
    uint64_t alufun = ADDQ;
    bool cc_set = false;
    e_dstE = ereg->getdstE()->getOutput();
    uint64_t eregIfun = ereg->getifun()->getOutput();
    uint64_t d_valA = regInst->readRegister(dreg->getrA()->getOutput(), error);

    
    //setting AluA
    aluA = setaluA(eregIcode, ereg);

    //setting aluB
    aluB = setaluB(eregIcode, ereg);

    //setting alufun
    alufun = setAluFun(eregIcode, eregIfun);

    //set cc
    cc_set = setCC(eregIcode);

    //set dstE
    e_cnd = takeCondition(eregIcode, eregIfun);
    e_dstE = setdstE(eregIcode, e_cnd, e_dstE);

    e_valE = ALU(eregIcode, eregIfun, aluA, aluB);

    if (cc_set) {
        e_cnd = set_cc(eregIcode, eregIfun, e_valE, aluA, aluB);
    }
    

    //set e_cnd
    if (eregIcode == ICMOVXX) {
        e_valE = ereg->getvalA()->getOutput();
    }

    if (eregIcode == IMRMOVQ || eregIcode == IIRMOVQ || eregIcode == IRMMOVQ) { 
        e_valE = ereg->getvalB()->getOutput() + ereg->getvalC()->getOutput();
    }


    setMInput(mreg, ereg->getstat()->getOutput(), eregIcode, e_dstE,
                ereg->getdstM()->getOutput(), ereg->getvalA()->getOutput(),
                e_cnd, e_valE);

}



void ExecuteStage::setMInput(M * mreg, uint64_t stat, uint64_t icode,
                            uint64_t e_dstE, uint64_t dstM, uint64_t valA, 
                            uint64_t Cnd, uint64_t e_valE) 
{
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getCnd()->setInput(Cnd);
    mreg->getvalE()->setInput(e_valE);
    mreg->getvalA()->setInput(valA);
    mreg->getdstE()->setInput(e_dstE);
    mreg->getdstM()->setInput(dstM);
}


void ExecuteStage::doClockHigh(PipeReg ** pregs)
{

    M * mreg = (M *) pregs[MREG];

    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getCnd()->normal();
    mreg->getvalE()->normal();
    mreg->getvalA()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
}

uint64_t ExecuteStage::gete_dstE()
{
    return e_dstE;
}

uint64_t ExecuteStage::gete_valE()
{
    return e_valE;
}

uint64_t ExecuteStage::setaluA(uint64_t eregIcode, E * ereg) {
    
    switch (eregIcode)
    {   
        case IRRMOVQ:
        case IOPQ: 
            return ereg->getvalA()->getOutput();
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
            return ereg->getvalC()->getOutput();
        case ICALL:
        case IPUSHQ:
            return -8;
        case IRET:
        case IPOPQ:
            return 8;
        default:
            return 0;
    }

    }
    /*
    if (eregIcode == IRRMOVQ || eregIcode == IOPQ) return ereg->getvalA()->getOutput();

    else if (eregIcode == IIRMOVQ || eregIcode == IRMMOVQ || 
                eregIcode == IMRMOVQ) return ereg->getvalC()->getOutput();

    else if (eregIcode == ICALL || eregIcode == IPUSHQ) return -8;

    else if (eregIcode == IRET || eregIcode == IPOPQ) return 8;

    return 0;
}
*/
uint64_t ExecuteStage::setaluB(uint64_t eregIcode, E * ereg){
    if (eregIcode == IRMMOVQ || eregIcode == IMRMOVQ || eregIcode == IOPQ || eregIcode == ICALL ||
            eregIcode == IPUSHQ || eregIcode == IRET || eregIcode == IPOPQ) {
            return ereg->getvalB()->getOutput();
            }
    else if (eregIcode == IRRMOVQ || eregIcode == IIRMOVQ) return 0;
    else return 0;
}

uint64_t ExecuteStage::setAluFun(uint64_t eregIcode, uint64_t eregIfun) {

        if (eregIcode == IOPQ) return eregIfun;
        return ADDQ;

}

bool ExecuteStage::setCC(uint64_t eregIcode)
{
    if (eregIcode == IOPQ) return true;
    else return false;
}

uint64_t ExecuteStage::setdstE(uint64_t eregIcode, uint64_t e_cnd, uint64_t e_dstE) {
    if (eregIcode == IRRMOVQ && !e_cnd) return RNONE;
    else return e_dstE;

}

uint64_t ExecuteStage::ALU(uint64_t eregIcode, uint64_t eregIfun,
                        uint64_t aluA, uint64_t aluB)
{
    if (eregIfun == SUBQ) {
        return aluB - aluA;
    }
    else if (eregIfun == ANDQ) {
        return aluA & aluB;
    }
    else if (eregIfun == XORQ) {
        return aluA ^ aluB;
    }
    return aluA + aluB;

    
}

//0 == negative , 1 == positive
uint64_t ExecuteStage::sign(uint64_t x) {
        uint64_t sign = x >> 63;
        return sign;
}

uint64_t ExecuteStage::set_cc(uint64_t eregIcode, uint64_t eregIfun, uint64_t total, uint64_t aluA, uint64_t aluB) {
        uint64_t CC = 0;
        bool error = false;
        uint64_t overflow = 0;
        ConditionCodes * codes = ConditionCodes::getInstance(); 
        //uint64_t zero = 0;
        //uint64_t negative = 0;
        //if (setCC(eregIcode) && eregIcode == 0x6) {
            if (eregIcode == IOPQ) {
                if (eregIfun == ADDQ) {
                    overflow = (sign(aluA) == sign(aluB)) && (sign(total) != sign(aluA));
                    //codes->setConditionCode(overflow, OF, error);
                }      
                else if (eregIfun == SUBQ) {
                    overflow = (sign(aluA) != sign(aluB)) && (sign(total) != sign(aluB));
                    //codes->setConditionCode(overflow, OF, error);

                }
            }
            else {
                overflow = 0;
            }
            codes->setConditionCode(overflow, OF, error);
            //if (overflow == 1) {
            //    codes->setConditionCode(true, OF, error);
            //}
            //else {
            //    codes->setConditionCode(false, OF, error);
            //}
            if (Tools::getBits(total, 63, 63) == 1) {
                codes->setConditionCode(true, SF, error);
            }
            else {
                codes->setConditionCode(false, SF, error);
            }
            if (total == 0) {
                codes->setConditionCode(true, ZF, error);
            }
            else {
                codes->setConditionCode(false, ZF, error);
            }
        //}
        return CC;
}


uint64_t ExecuteStage::takeCondition(uint64_t e_icode, uint64_t e_ifun) {
    bool error = false;
    ConditionCodes * codes = ConditionCodes::getInstance(); 
    uint64_t overflow = codes->getConditionCode(OF, error);
    uint64_t zero = codes->getConditionCode(ZF, error);
    uint64_t sign = codes->getConditionCode(SF, error);


    if (e_icode == IJXX || e_icode == ICMOVXX){// return 0;

    if (e_ifun == UNCOND) return 1;

    else if (e_ifun == LESSEQ) return (sign ^ overflow) | zero;

    else if (e_ifun == LESS) return sign ^ overflow;

    else if (e_ifun == EQUAL) return zero;

    else if (e_ifun == NOTEQUAL) return !zero;

    else if (e_ifun == GREATEREQ) return !(sign ^ overflow);

    else if (e_ifun == GREATER) return !(sign ^ overflow) & !zero;
    }
    else return 0;
}
