#include <iostream>
#include <iomanip>
#include "ConditionCodes.h"
#include "Tools.h"

//cc_instance will be initialized to reference the single 
//instance of ConditionCodes
ConditionCodes * ConditionCodes::ccInstance = NULL;

/**
 * ConditionCodes constructor
 * initialize the codes field to 0
 */
ConditionCodes::ConditionCodes()
{
    codes = 0;
}

/**
 * getInstance
 * if ccInstance is NULL then getInstance creates the
 * single ConditionCodes instance and sets ccInstance
 * to that. Otherwise, it returns ccInstance
 *
 * @return ccInstance
 */
ConditionCodes * ConditionCodes::getInstance()
{
   if (ccInstance == NULL) ccInstance = new ConditionCodes;
   return ccInstance;
}

/*
 * getConditionCode
 * accepts a condition code number (OF, SF, or ZF) and returns 
 * the value of the condition code 
 *
 * @param ccNum equal to either OF, SF, or ZF
 * @return the value of bit ccNum out of codes. if ccNum is 
 *         out of range then returns false
 * @return error is set to true if ccNum is out of range and
 *         false otherwise
 */
bool ConditionCodes::getConditionCode(int32_t ccNum, bool & error)
{
   Tools t = Tools();
   if (ccNum != 3 && ccNum != 6 && ccNum != 2) {
       error = true;
       return false;
   } else {
       error = false;
       uint64_t bit = t.getBits(codes, ccNum, ccNum);
       if (bit == 0) return false;       
       else return true;
   }
}

/*
 * setConditionCode
 * accepts a condition code number (OF, SF, or ZF) and value
 * (true or false) and sets the condition code bit to that
 * value (1/true or 0/false). if the ccNum value is out of 
 * range then codes does not get modified. 
 *
 * @param value to set the condition code bit to (true/1 or false/0)
 * @param ccNum condition code number, either OF, SF, or ZF
 * @return error is set to true if ccNum is out of range and
 *         false otherwise
 */
void ConditionCodes::setConditionCode(bool value, int32_t ccNum, 
                                      bool & error)
{
   Tools t = Tools();
   switch (ccNum)
   {
       case 3:
       case 6:
       case 2:
        error = false;
        if (value) codes = t.setBits(codes, ccNum, ccNum);
        else codes =  t.clearBits(codes, ccNum, ccNum);
        break;
       default:
        error = true;
   }
   return;
}

/*
 * dump
 * outputs the values of the condition codes
 */
void ConditionCodes::dump()
{
   int32_t zf = Tools::getBits(codes, ZF, ZF);
   int32_t sf = Tools::getBits(codes, SF, SF);
   int32_t of = Tools::getBits(codes, OF, OF);
   std::cout << std::endl;
   std::cout << "ZF: " << std::hex << std::setw(1) << zf << " ";
   std::cout << "SF: " << std::hex << std::setw(1) << sf << " ";
   std::cout << "OF: " << std::hex << std::setw(1) << of << std::endl;
}
