//class to perform the combinational logic of
//the Decode stage
class DecodeStage: public Stage
{
   private:
      uint64_t d_srcA;
      uint64_t d_srcB;
      void setEInput(E * ereg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t valC, uint64_t valA,
                           uint64_t valB, uint64_t dstE, uint64_t dstM,
                           uint64_t srcA, uint64_t srcB);
      uint64_t setsrcA(uint64_t dregIcode, D * dreg);
      uint64_t setsrcB(uint64_t dregIcode, D * dreg);
      uint64_t setdstE(uint64_t dregIcode, D * dreg);
      uint64_t setdstM(uint64_t dregIcode, D * dreg);
      uint64_t selFwdA(uint64_t srcA, uint64_t e_dstE, uint64_t e_valE,
          M * mreg, W * wreg, D * dreg, MemoryStage * mStage);
      uint64_t FwdB(uint64_t srcB, uint64_t e_dstE, uint64_t e_valE,
          M * mreg, W * wreg, D * dreg, MemoryStage * mStage);
        




   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);
      uint64_t getd_srcA();
      uint64_t getd_srcB();

};