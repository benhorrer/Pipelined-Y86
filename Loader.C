#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "Loader.h"
#include "Memory.h"

#define ADDRBEGIN 2
#define ADDREND 4
#define DATABEGIN 7
#define COMMENT 28

/* 
 * Loader
 * opens up the file named in argv[0] and loads the
 * contents into memory. If the file is able to be loaded,
 * then loaded is set to true.
 */
//This method is complete and does not need to be modified.  
Loader::Loader(int argc, char * argv[])
{
   std::ifstream inf;  //input file stream for reading from file
   int lineNumber = 1;
   lastAddress = -1;
   loaded = false;

   //if no file name given or filename badly formed, return without loading
   if (argc < 2 || badFile(argv[1])) return;  
   inf.open(argv[1]);

   //if file can't be opened, return without loading
   if (!inf.is_open()) return;  
  
   std::string line;
   while (getline(inf, line))
   {
      if (hasErrors(line))
      {
         std::cout << "Error on line " << std::dec << lineNumber 
              << ": " << line << std::endl;
         return;
      }
      if (hasAddress(line) && hasData(line)) loadLine(line);
      lineNumber++;
   }
   loaded = true;
}

/*
 * hasAddress
 * returns true if the line passed in has an address on it.
 * A line that has an address has a '0' in column 0.
 * It is assumed that the address has already been checked to
 * make sure it is properly formed.
 *
 * @param line - a string containing a line of valid input from 
 *               a .yo file
 * @return true, if the line has an address on it
 *         false, otherwise
 */
bool Loader::hasAddress(std::string line)
{
   if (line != "" && ((int) line.length() >= 5) && line[0] == '0') {
      return true;
   }
   return false;
}

/*
 * hasData
 * returns true if the line passed in has data on it.
 * A line that has data does not contain a space
 * at index DATABEGIN.
 * It is assumed that the data has already been checked to
 * make sure it is properly formed.
 *
 * @param line - a string containing a line of valid input from 
 *               a .yo file
 * @return true, if the line has data in it
 *         false, otherwise
 */
bool Loader::hasData(std::string line)
{
   if (line.at(DATABEGIN) != ' ') return true;
   return false;
}

/*
 * hasComment
 * returns true if line is at least COMMENT in length and 
 * line has a | at index COMMENT.
 * 
 * @param line - a string containing a line from a .yo file
 * @return true, if the line is long enough and has a | in index COMMENT
 *         false, otherwise
 */
bool Loader::hasComment(std::string line)
{
    if (line.length() >= COMMENT && line.at(COMMENT) == '|') {
        return true;
    }
    return false;
}

/*
 * loadLine
 * The line that is passed in contains an address and data.
 * This method loads that data into memory byte by byte
 * using the Memory::getInstance->putByte method.
 *
 * @param line - a string containing a line of valid input from 
 *               a .yo file. The line contains an address and
 *               a variable number of bytes of data (at least one)
 */
void Loader::loadLine(std::string line)
{
   bool errors = false;
   lastAddress = convert(line, ADDRBEGIN, 3);
   int32_t currentByte = DATABEGIN;
   while(line[currentByte] != ' ') {
      Memory::getInstance()->putByte(convert(line, currentByte, 2), lastAddress, errors);
      currentByte += 2;
      lastAddress++;
   }
}

/*
 * convert
 * takes "len" characters from the line starting at character "start"
 * and converts them to a number, assuming they represent hex characters.
 * For example, if len is 2 and line[start] is '1' and 
 * line[start + 1] is 'a' then this function returns 26.
 * This function assumes that the line is long enough to hold the desired
 * characters and that the characters represent hex values.
 *
 * @param line - string of characters
 * @param start - starting index in line
 * @param len - represents the number of characters to retrieve
 */
int32_t Loader::convert(std::string line, int32_t start, int32_t len)
{
   char _line[len];
   for (int i = start, j = 0; j < len; i++, j++) {
      _line[j] = line[i];
   } 
   return strtol(_line, NULL, 16);
}

/*
 * hasErrors
 * Returns true if the line file has errors in it and false
 * otherwise.
 *
 * @param line - a string that contains a line from a .yo file
 * @return true, if the line has errors 
 *         false, otherwise
 */
bool Loader::hasErrors(std::string line)
{
   if (!hasComment(line)) return true;
   if (!hasAddress(line)) {
       return !isSpaces(line, 0, (COMMENT - 1));
   }
   if (errorAddr(line)) return true;
   if (!hasData(line)) {
       return !isSpaces(line, DATABEGIN, (COMMENT - 1));
   }
   int count = 1;
   for (int i = 8; line[i] != ' '; i++) {
      count++;
   }
   if (errorData(line, count)) return true;
   int32_t currentAddr = convert(line, ADDRBEGIN, 3);
   if (currentAddr < lastAddress) return true;
   if (currentAddr + (count / 2) > MEMSIZE) return true;
   
   return false;
}

/*
 * errorData
 * Called when the line contains data. It returns true if the data
 * in the line is invalid. 
 *
 * Valid data consists of characters in the range
 * '0' .. '9','a' ... 'f', and 'A' .. 'F' (valid hex digits). 
 * The data digits start at index DATABEGIN.
 * The hex digits come in pairs, thus there must be an even number of them.
 * In addition, the characters after the last hex digit up to the
 * '|' character at index COMMENT must be spaces. 
 * If these conditions are met, errorData returns false, else errorData
 * returns true.
 *
 * @param line - input line from the .yo file
 * @return numDBytes is set to the number of data bytes on the line
 */
bool Loader::errorData(std::string line, int32_t & numDBytes)
{
   if ((numDBytes % 2) != 0) return true;
   for (int32_t itr = DATABEGIN; itr < (DATABEGIN + (numDBytes)); itr++) {
       if (!isxdigit(line[itr])) return true;
   }
   if (!isSpaces(line, (DATABEGIN + (numDBytes)), (COMMENT - 1))) return true;
   return false;
}

/*
 * errorAddr
 * This function is called when the line contains an address in order
 * to check whether the address is properly formed.  An address must be of
 * this format: 0xHHH: where HHH are valid hex digits.
 * 
 * @param line - input line from a .yo input file
 * @return true if the address is not properly formed and false otherwise
 */
bool Loader::errorAddr(std::string line)
{
   if (line[0] != '0' || line[1] != 'x') return true;
   if (line[ADDREND + 1] != ':' || line[ADDREND + 2] != ' ') return true;
   if (isxdigit(line[ADDRBEGIN]) && isxdigit(line[ADDRBEGIN + 1]) && 
   isxdigit(line[ADDREND])) {
       return false;
   }
   return true;
}

/* 
 * isSpaces
 * This function checks that characters in the line starting at 
 * index start and ending at index end are all spaces.
 * This can be used to check for errors
 *
 * @param line - string containing a line from a .yo file
 * @param start - starting index
 * @param end - ending index
 * @return true, if the characters in index from start to end are spaces
 *         false, otherwise
 */
bool Loader::isSpaces(std::string line, int32_t start, int32_t end)
{
    for (int32_t itr = start; itr <= end; itr++) {
        if (line.at(itr) != ' ') return false;
    }
    return true;        
}

/*
 * isLoaded
 * getter for the private loaded data member
 */
bool Loader::isLoaded()
{
   return loaded;  
}

/*
 * badFile
 * returns true if the name of the file passed in is an improperly 
 * formed .yo filename. A properly formed .yo file name is at least
 * four characters in length and ends with a .yo extension.
 *
 * @return true - if the filename is improperly formed
 *         false - otherwise
 */
bool Loader::badFile(std::string filename)
{
   std::ifstream exists(filename);
   int strLength = filename.length();
   if (strLength < 4) return true;
   else if (exists.fail()) return true;
   else if (filename.at(strLength-3) != '.' 
      || filename.at(strLength-2) != 'y' 
      || filename.at(strLength-1) != 'o') return true;
   return false;
}
