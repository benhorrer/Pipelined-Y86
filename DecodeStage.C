<<<<<<< HEAD
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

   if (dreg->get)

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
   if (dregIcode == 6) {
      valA = dreg->getrA()->getOutput();
      valB = dreg->getrA()->getOutput();
      regA = dreg->getrA()->getOutput();
      regB = dreg->getrB()->getOutput();
   }
   
   if (dregIcode == 0x0 || dregIcode == 0x1 || dregIcode == 0x9) {
         regA = NULL;
         regB = NULL;
         dstE = NULL;
         dstM = NULL;
         valA = NULL;
         valB = NULL;
         srcA = NULL;
         srcB = NULL;
      }

   else if (dregIcode == 0x2 || dregIcode == 0x4 || 
            dregIcode == 0x5 || dregIcode == 0x6) {
               regA = dreg->getrA()->getOutput();
               regB = dreg->getrB()->getOutput();
      }

   else if (dregIcode == 0x3) {
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

=======
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
#include "Instructions.h"

bool DecodeStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   E * ereg = (E *) pregs[EREG];
   D * dreg = (D *) pregs[DREG];
   M * mreg = (M *) pregs[MREG];
   W * wreg = (W *) pregs[WREG];
   uint64_t valA = 0, valB = 0;
   uint64_t dstE = RNONE, dstM = RNONE, srcA = RNONE, srcB = RNONE;
   RegisterFile * regInst = RegisterFile::getInstance();

   uint64_t dregIcode = dreg->geticode()->getOutput();
   // set srcA
   if (dregIcode == IRRMOVQ || dregIcode == IRMMOVQ ||
       dregIcode == IOPQ || dregIcode == IPUSHQ)
   {
       srcA = dreg->getrA()->getOutput();
   }
   else if (dregIcode == IPOPQ || dregIcode == IRET)
   {
       srcA = 4;
   }
   // set srcB
   if (dregIcode == IOPQ || dregIcode == IRMMOVQ ||
       dregIcode == IMRMOVQ)
   {
       srcB = dreg->getrB()->getOutput();
   }
   else if (dregIcode == IPUSHQ || dregIcode == IPOPQ || 
            dregIcode == ICALL || dregIcode == IRET)
   {
       srcB = 4;
   }
   // set dstE
   if (dregIcode == IRRMOVQ || dregIcode == IIRMOVQ ||
       dregIcode == IOPQ)
   {
       dstE = dreg->getrB()->getOutput();
   } 
   else if (dregIcode == IPUSHQ || dregIcode == IPOPQ || 
            dregIcode == ICALL || dregIcode == IRET)
   {
       dstE = 4;
   }
   // set dstM
   if (dregIcode == IMRMOVQ || dregIcode == IPOPQ)
   {
       dstM = dreg->getrA()->getOutput();
   }
   // Sel + FwdA 
    if (srcA == ereg->getdstE()->getOutput()) valA = ereg->getvalC()->getOutput();
    else if (srcA == mreg->getvalE()->getOutput()) valA = mreg->getvalE()->getOutput();
    else if (srcA == wreg->getdstE()->getOutput()) valA = wreg->getvalE()->getOutput();
    else {
        bool selError = false;
        valA = regInst->readRegister(dreg->getrA()->getOutput(), selError);
    }

   //FwdB
     if (srcB == ereg->getdstE()->getOutput()) valB = ereg->getvalC()->getOutput();
    else if (srcB == mreg->getvalE()->getOutput()) valB = mreg->getvalE()->getOutput();
    else if (srcB == wreg->getdstE()->getOutput()) valB = wreg->getvalE()->getOutput();
    else {
        bool selError = false;
        valB = regInst->readRegister(dreg->getrB()->getOutput(), selError);
    }

   setEInput(ereg, dreg->getstat()->getOutput(), dregIcode,
               dreg->getifun()->getOutput(), dreg->getvalC()->getOutput(),
               valA, valB, dstE, dstM, srcA, srcB);
    

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

>>>>>>> 74b526abdc6491e4b213553562ac56fa00986d87
