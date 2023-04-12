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

    uint64_t Cnd = 0, valE = 0;

    uint64_t eregIcode = ereg->geticode()->getOutput();

    valE = ereg->getvalC()->getOutput();
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

    


    setMInput(mreg, ereg->getstat()->getOutput(), eregIcode, ereg->getdstE()->getOutput(),
                ereg->getdstM()->getOutput(), ereg->getvalA()->getOutput(),
                Cnd, valE);

}

//false == negative , true == positive
bool sign(uint64_t x) {
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

