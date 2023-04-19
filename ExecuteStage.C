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


bool ExecuteStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    E * ereg = (E *) pregs[EREG];
    M * mreg = (M *) pregs[MREG];
    D * dreg = (D *) pregs[DREG];

    uint64_t Cnd = 0, valE = 0;
    bool error = false;
    uint64_t eregIcode = ereg->geticode()->getOutput();
<<<<<<< HEAD
    uint64_t aluA;
    uint64_t aluB;
    uint64_t alufun;
    bool cc_set;
    uint64_t dstE;
    uint64_t eregIfun = ereg->getifun()->getOutput();
    uint64_t cnd = 0x000;
    uint64_t OF;

    if (eregIcode == IRRMOVQ || eregIcode == IOPQ) aluA = ereg->getvalA()->getOutput();
    else if (eregIcode == IRRMOVQ || eregIcode == IRMMOVQ || 
                eregIcode == IMRMOVQ) ereg->getvalC()->getOutput();
    else if (eregIcode == ICALL || eregIcode == IPUSHQ) aluA = -8;
    else if (eregIcode == IRET || eregIcode == IPOPQ) aluA = 8;
    else aluA = 0;

    if (eregIcode == IRMMOVQ || eregIcode == IMRMOVQ || eregIcode == IOPQ || eregIcode == ICALL ||
            eregIcode == IPUSHQ || eregIcode == IRET || eregIcode == IPOPQ) {
                aluB = ereg->getvalB()->getOutput();
            }
    else if (eregIcode == IRRMOVQ || eregIcode == IIRMOVQ) aluB = 0;
    else aluB = 0; 

    if (eregIcode == IOPQ) alufun = eregIfun;
    else alufun = ADDQ;

    if (eregIcode == IOPQ) cc_set = true;
    else cc_set = false;

    if (eregIcode == IRRMOVQ && true) cc_set = true;
    else cc_set = false;

    if (cc_set && eregIcode == 0x6) {
        if (eregIfun == 0) {
                OF = (sign(aluA) == sign(aluB)) && (sign(aluA+aluB) != sign(aluA));
        }
        else if (eregIfun == 1) {
            OF = (sign(aluA) != sign(aluB)) && (sign(aluA-aluB) != sign(aluA));
        }
        else if (eregIfun == 2 || eregIfun == 3) {
            OF = 0;
        }
    }

    
=======
    RegisterFile * regInst = RegisterFile::getInstance();
    valE = ereg->getvalC()->getOutput();
    uint64_t aluA = 0;
    uint64_t aluB = 0;
    uint64_t alufun = ADDQ;
    bool cc_set;
    uint64_t dstE = ereg->getdstE()->getOutput();
    uint64_t eregIfun = ereg->getifun()->getOutput();
    uint64_t e_cnd = 0x000;
    uint64_t OF;
    uint64_t d_valA = regInst->readRegister(dreg->getrA()->getOutput(), error);

    
    //setting AluA
    if (eregIcode == IRRMOVQ || eregIcode == IOPQ) aluA = ereg->getvalA()->getOutput();
    else if (eregIcode == IIRMOVQ || eregIcode == IRMMOVQ || 
                eregIcode == IMRMOVQ) aluA = ereg->getvalC()->getOutput();
    else if (eregIcode == ICALL || eregIcode == IPUSHQ) aluA = -8;
    else if (eregIcode == IRET || eregIcode == IPOPQ) aluA = 8;

    //setting aluB
    if (eregIcode == IRMMOVQ || eregIcode == IMRMOVQ || eregIcode == IOPQ || eregIcode == ICALL ||
            eregIcode == IPUSHQ || eregIcode == IRET || eregIcode == IPOPQ) {
                aluB = ereg->getvalB()->getOutput();
            }
    else if (eregIcode == IRRMOVQ || eregIcode == IIRMOVQ) aluB = 0;


    //setting alufun
    if (eregIcode == IOPQ) alufun = eregIfun;
    else alufun = ADDQ;

    if (eregIcode == IOPQ) cc_set = true;
    else cc_set = false;

    //set cc
    if (eregIcode == IRRMOVQ) cc_set = true;
    else cc_set = false;

    //set dstE
    if (eregIcode == IRRMOVQ && !e_cnd) dstE = RNONE;

    if (cc_set && eregIcode == 0x6) {
        if (eregIfun == 0) {
                OF = (sign(aluA) == sign(aluB)) && (sign(aluA+aluB) != sign(aluA));
        }
        else if (eregIfun == 1) {
            OF = (sign(aluA) != sign(aluB)) && (sign(aluA-aluB) != sign(aluA));
        }
        else if (eregIfun == 2 || eregIfun == 3) {
            OF = 0;
        }
    }
    
>>>>>>> 74b526abdc6491e4b213553562ac56fa00986d87


    setMInput(mreg, ereg->getstat()->getOutput(), eregIcode, ereg->getdstE()->getOutput(),
                ereg->getdstM()->getOutput(), ereg->getvalA()->getOutput(),
                Cnd, valE);

}

//false == negative , true == positive
<<<<<<< HEAD
bool sign(uint64_t x) {
=======
bool ExecuteStage::sign(uint64_t x) {
>>>>>>> 74b526abdc6491e4b213553562ac56fa00986d87
        if (x >= 0) {
            return true;
        }

        else if (x < 0) return false;
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

    M * mreg = (M *) pregs[MREG];

    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getCnd()->normal();
    mreg->getvalE()->normal();
    mreg->getvalA()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
}

