//class to perform the combinational logic of
//the Fetch stage
class ExecuteStage: public Stage
{
   private: 
      void setMInput(M * mreg, uint64_t stat, uint64_t icode,
                            uint64_t dstE, uint64_t dstM, uint64_t valA, 
                            uint64_t Cnd, uint64_t valE);
      bool sign(uint64_t x);

   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

};
