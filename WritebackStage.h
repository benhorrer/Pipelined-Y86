//class to perform the combinational logic of
//the Fetch stage
class WritebackStage: public Stage
{
   private:
      uint64_t w_stat;
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);
      uint64_t getw_stat();

};
