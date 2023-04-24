//class to perform the combinational logic of
//the Fetch stage
class ExecuteStage: public Stage
{
   private:
      uint64_t e_dstE, e_valE;
      void setMInput(M * mreg, uint64_t stat, uint64_t icode,
                            uint64_t dstE, uint64_t dstM, uint64_t valA, 
                            uint64_t Cnd, uint64_t valE);
      uint64_t sign(uint64_t x);
      uint64_t setaluA(uint64_t eregIcode, E * ereg);
      uint64_t setaluB(uint64_t eregIcode, E * ereg);
      uint64_t setAluFun(uint64_t eregIcode, uint64_t eregIfun);
      bool setCC(uint64_t eregIcode);
      uint64_t setdstE(uint64_t eregIcode, uint64_t e_cnd, uint64_t e_dstE);
      uint64_t ALU(uint64_t eregIcode, uint64_t eregIfun,
                        uint64_t aluA, uint64_t aluB);
      uint64_t set_cc(uint64_t eregIcode, uint64_t eregIfun, uint64_t total, uint64_t aluA, uint64_t aluB);

   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);
      uint64_t gete_dstE();
      uint64_t gete_valE();

};
