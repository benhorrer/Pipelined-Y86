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
#include "DecodeStage.h"
#include "Status.h"
#include "Debug.h"

bool DecodeStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   E * ereg = (E *) pregs[EREG];
   D * dreg = (D *) pregs[DREG];
   uint64_t valA;
   uint64_t valB;
   uint64_t dstE;
   uint64_t dstM;
   uint64_t srcA;
   uint64_t srcB;
   

   uint64_t dregIcode = dreg->geticode()->getOutput();
   valA = 0;
   valB = 0;
   dstE = RNONE;
   dstM = RNONE;
   srcA = RNONE;
   srcB = RNONE;

   setEInput(ereg, dreg->getstat()->getOutput(), dregIcode,
               dreg->getifun()->getOutput(), dreg->getvalC()->getOutput(),
               valA, valB, dstE, dstM, srcA, srcB);

   /*
   if (0x60 <= dregIcode <= 0x63) {
      valA = dreg->getrA()->getOutput();
      valB = dreg->getrA()->getOutput();
      regA = dreg->getrA()->getOutput();
      regB = dreg->getrB()->getOutput();
   }
   
   if (dregIcode == 0x00 || dregIcode == 0x10 || dregIcode == 0x90) {
         regA = NULL;
         regB = NULL;
         dstE = NULL;
         dstM = NULL;
         valA = NULL;
         valB = NULL;
         srcA = NULL;
         srcB = NULL;
      }

   else if ((0x20 <= dregIcode <= 0x26) || dregIcode == 0x40 || 
            dregIcode == 0x50 || (0x60 <= dregIcode <= 0x63)) {
               regA = dreg->getrA()->getOutput();
               regB = dreg->getrB()->getOutput();
      }

   else if (dregIcode == 0x30) {
         regA = 0xF;
         regB = dreg->getrB()->getOutput();
      }

   else {
      regA = dreg->getrA()->getOutput();
   }
   */
    

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

