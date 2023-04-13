//class to perform the combinational logic of
//the Fetch stage
class FetchStage: public Stage
{
   private:
      void setDInput(D * dreg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                     uint64_t rA, uint64_t rB,
                     uint64_t valC, uint64_t valP);
      uint64_t selectPC(F * freg, M * mreg, W * wreg);
      uint64_t PCIncrement(uint64_t f_pc, bool f_needRegIds, bool f_needValC);
      uint64_t predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP);
      bool needRegIds(uint64_t f_icode);
      uint64_t getRegIds(uint64_t f_pc);
      uint64_t buildValC();
      bool needValC(uint64_t f_icode);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);


};
