//class to perform the combinational logic of
//the Fetch stage
class FetchStage: public Stage
{
   private:
      bool stallF;
      bool stallD;
      void setDInput(D * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      uint64_t selectPC(F * freg, M * mreg, W * wreg);
      uint64_t PCIncrement(uint64_t f_pc, bool f_needRegIds, bool f_needValC);
      uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);
      bool needRegIds(uint64_t f_icode);
      uint64_t getRegIds(uint64_t f_pc);
      uint64_t buildValC(uint64_t f_pc, uint64_t f_icode);
      bool needValC(uint64_t f_icode);
      bool instr_valid(uint64_t f_icode);
      uint64_t f_stat(uint64_t f_icode, MemoryStage * mStage);
      bool f_stall(uint64_t e_icode, uint64_t e_dstM, 
          uint64_t d_srcA, uint64_t d_srcB);
      bool d_stall(uint64_t e_icode, uint64_t e_dstM, 
          uint64_t d_srcA, uint64_t d_srcB);
      void calculateControlSignals(uint64_t e_icode, uint64_t e_dstM,
          uint64_t d_srcA, uint64_t d_srcB);


   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);


};
