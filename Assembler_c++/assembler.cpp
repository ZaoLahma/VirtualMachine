#include "./assembler.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include "../inc/InstructionSet.h"

Assembler::Assembler(const char* fileBuf, const unsigned int fileSize) :
outFileBufSize(0),
fileBufPtr(fileBuf),
fileSize(fileSize)
{

}

void crash()
{
    char* crash = NULL;
    *crash = 0;
}

std::string& Assembler::trim_right(std::string& s, const std::string& delimiters)
{
  return s.erase( s.find_last_not_of( delimiters ) + 1 );
}

std::string& Assembler::trim_left(std::string& s, const std::string& delimiters)
{
  return s.erase( 0, s.find_first_not_of( delimiters ) );
}

std::string& Assembler::trim(std::string& s, const std::string& delimiters)
{
  return trim_left( trim_right(s, delimiters), delimiters);
}

char convertStringToInstruction(const char* instruction, Instruction& instructionRef)
{
    printf("Instruction: %s\n", instruction);
    if(strcmp(instruction, "nop") == 0)
    {
        instructionRef.instruction = NOP;
        instructionRef.instructionSize = 1;
        return NOP;
    }
    if(strcmp(instruction, "push") == 0)
    {
        instructionRef.instruction = PUSH;
        instructionRef.instructionSize = 5;
        return PUSH;
    }
    if(strcmp(instruction, "pop") == 0)
    {
        instructionRef.instruction = POP;
        instructionRef.instructionSize = 1;
        return POP;
    }
    if(strcmp(instruction, "load") == 0)
    {
        instructionRef.instruction = LOAD;
        instructionRef.instructionSize = 2;
        return LOAD;
    }
    if(strcmp(instruction, "store") == 0)
    {
        instructionRef.instruction = STORE;
        instructionRef.instructionSize = 2;
        return STORE;
    }
    if(strcmp(instruction, "jmp") == 0)
    {
        instructionRef.instruction = JMP;
        instructionRef.instructionSize = 3;
        return JMP;
    }
    if(strcmp(instruction, "jz") == 0)
    {
        instructionRef.instruction = JZ;
        instructionRef.instructionSize = 3;
        return JZ;
    }
    if(strcmp(instruction, "jnz") == 0)
    {
        instructionRef.instruction = JNZ;
        instructionRef.instructionSize = 3;
        return JNZ;
    }
    if(strcmp(instruction, "add") == 0)
    {
        instructionRef.instruction = ADD;
        instructionRef.instructionSize = 1;
        return ADD;
    }
    if(strcmp(instruction, "sub") == 0)
    {
        instructionRef.instruction = SUB;
        instructionRef.instructionSize = 1;
        return SUB;
    }
    if(strcmp(instruction, "mul") == 0)
    {
        instructionRef.instruction = MUL;
        instructionRef.instructionSize = 1;
        return MUL;
    }
    if(strcmp(instruction, "div") == 0)
    {
        instructionRef.instruction = DIV;
        instructionRef.instructionSize = 1;
        return DIV;
    }
    if(strcmp(instruction, "print") == 0)
    {
        instructionRef.instruction = PRINT;
        instructionRef.instructionSize = 1;
        return PRINT;
    }
    if(strcmp(instruction, "stop") == 0)
    {
        instructionRef.instruction = STOP;
        instructionRef.instructionSize = 1;
        return STOP;
    }
    if(strcmp(instruction, "call") == 0)
    {
        return CALL;
    }
    if(strcmp(instruction, "ret") == 0)
    {
        return RET;
    }

    printf("ERROR FATAL: Could not convert %s to instruction\n", instruction);

    crash();

    return 0xff;
}

inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0);
}

void Assembler::tryGetLabel(const std::string str, Instruction& instruction)
{
    std::string firstWord = "";
    if(str.size())
    {
        for(unsigned int i = 0; i < str.size(); ++i)
        {
            if(str[i] != ' ' && str[i] != '\n')
            {
                firstWord+=str[i];
            }
            else
            {
                break;
            }
        }
    /*
        unsigned int index = 0;
        while(str[index] != ' ' && str[index] != '\n')
        {
            firstWord+=str[index];
            index++;
        }
        */
    }

    if(std::string::npos == firstWord.find("lbl_"))
    {
        return;
    }

    instruction.label = firstWord;
    std::cout<<"Is integer. Label: "<<instruction.label<<std::endl;
}

unsigned int getInstructionAddress(const std::vector<Instruction>& instructionVector)
{
    unsigned int retVal = 0;
    for(unsigned int i = 0; i < instructionVector.size(); ++i)
    {
        //std::cout<<"instruction: "<<+instructionVector[i].instruction<<" size: "<<instructionVector[i].instructionSize<<std::endl;
        retVal += instructionVector[i].instructionSize;
    }

    std::cout<<"Returning address: "<<retVal<<std::endl;

    return retVal;
}

std::string cleanUpMultipleSpaces(std::string& str)
{
    std::string retVal = "";
    if(str.size())
    {
        retVal += str[0];
        for(unsigned int i = 1; i < str.size(); ++i)
        {
            if(false == (str[i - 1] == ' ' && str[i] == ' '))
            {
               retVal += str[i];
            }
        }
    }
    return retVal;
}

void Assembler::tryGetInstruction(const std::string str, Instruction& instruction)
{
    size_t n = std::count(str.begin(), str.end(), ' ');

    std::cout<<"str: "<<str<<std::endl;
    std::cout<<"Number of spaces: "<<n<<std::endl;

    std::string instructionStr = str;

    if(n == 2 || instruction.label != "")
    {
        std::cout<<"label: "<<instruction.label<<std::endl;
        instructionStr = instructionStr.substr(instructionStr.find_first_of(" \t")+1);
        std::cout<<"instructionStr: "<<instructionStr<<std::endl;
        instructionStr = instructionStr.substr(0, instructionStr.find_first_of(" \t"));
        std::cout<<"instructionStr: "<<instructionStr<<std::endl;
    }
    else
    {
        instructionStr = instructionStr.substr(0, instructionStr.find_first_of(" \t"));
    }

    trim(instructionStr);

    instruction.instructionString = instructionStr;

    convertStringToInstruction(instructionStr.c_str(), instruction);
}

void Assembler::tryGetArgument(const std::string str, Instruction& instruction)
{
    std::string argumentString = str;

    std::cout<<"argumentString: "<<argumentString<<std::endl;

    if(instruction.instructionSize == 1)
    {
        std::cout<<"No arguments"<<std::endl;
        return;
    }
    argumentString = argumentString.substr(argumentString.find_last_of(" \t"));

    trim(argumentString);

    std::cout<<"argumentString processed: "<<argumentString<<std::endl;

    instruction.argumentsString = argumentString;

    /*
    unsigned int arguments = atoi(argumentString.c_str());

    std::cout<<"Arguments: "<<arguments<<std::endl;
    printf("Arguments hex: 0x%x\n", arguments);
    std::cout<<"(arguments & 0x0000ff00): "<<(arguments & 0x0000ff00)<<std::endl;

    instruction.arguments[3] = (uint8_t)((arguments & 0xff000000) >> 24);
    instruction.arguments[2] = (uint8_t)((arguments & 0x00ff0000) >> 16);
    instruction.arguments[1] = (uint8_t)((arguments & 0x0000ff00) >> 8);
    instruction.arguments[0] = (uint8_t)(arguments & 0x000000ff);

    std::cout<<"instruction.arguments[2]: "<<+instruction.arguments[2]<<std::endl;
    std::cout<<"instruction.arguments[3]: "<<+instruction.arguments[3]<<std::endl;
    */

    instruction.argumentsSize = instruction.instructionSize - 1;
}

void Assembler::assemble()
{
    std::string row;
    std::vector<std::string> rows;
    bool skipLine = false;
    for(unsigned int i = 0; i < fileSize; ++i)
    {
        if(fileBufPtr[i] != '\n')
        {
            if(fileBufPtr[i] == '#')
            {
                skipLine = true;
            }
            if(false == skipLine)
            {
                row += fileBufPtr[i];
                std::cout<<"Adding string: "<<row<<std::endl;
            }
        }
        else
        {
            if(row.size() != 0)
            {
                rows.push_back(row);
            }
            row = "";
            skipLine = false;
        }
    }

    for(unsigned int i = 0; i < rows.size(); ++i)
    {
        trim(rows[i]);
        rows[i] = cleanUpMultipleSpaces(rows[i]);
        std::cout<<"Trimmed: "<<rows[i]<<std::endl;
    }

    std::vector<Instruction> instructionsVector;

    for(unsigned int i = 0; i < rows.size(); ++i)
    {
        Instruction instruction;
        tryGetLabel(rows[i], instruction);
        tryGetInstruction(rows[i], instruction);
        tryGetArgument(rows[i], instruction);
        instruction.address = getInstructionAddress(instructionsVector);
        instructionsVector.push_back(instruction);
    }

    std::cout<<"Converting labels to addresses. instructionsVector.size(): "<<instructionsVector.size()<<std::endl;

    for(unsigned int i = 0; i < instructionsVector.size(); ++i)
    {
        if(instructionsVector[i].instruction == JMP ||
           instructionsVector[i].instruction == JZ ||
           instructionsVector[i].instruction == JNZ)
        {
            bool lookupSuccessful = false;
            for(unsigned int n = 0; n < instructionsVector.size(); ++n)
            {
                if(i != n)
                {
                    if(instructionsVector[i].argumentsString == instructionsVector[n].label)
                    {
                        unsigned int address = instructionsVector[n].address;
                        std::cout<<"Giving address "<<address<<" to instruction: "<<instructionsVector[i].instructionString<<" for label: "<<instructionsVector[i].argumentsString<<std::endl;
                        instructionsVector[i].arguments[3] = (uint8_t)((address & 0xff000000) >> 24);
                        instructionsVector[i].arguments[2] = (uint8_t)((address & 0x00ff0000) >> 16);
                        instructionsVector[i].arguments[1] = (uint8_t)((address & 0x0000ff00) >> 8);
                        instructionsVector[i].arguments[0] = (uint8_t)(address & 0x000000ff);
                        lookupSuccessful = true;
                    }
                }
            }
            if(!lookupSuccessful)
            {
                std::cout<<"ERROR FATAL: Lookup of label: "<<instructionsVector[i].argumentsString<<" for instruction: "<<instructionsVector[i].instructionString<<" failed."<<std::endl;
                crash();
            }
        }
        else
        {
            unsigned int arguments = atoi(instructionsVector[i].argumentsString.c_str());

            std::cout<<"Arguments: "<<arguments<<std::endl;
            printf("Arguments hex: 0x%x\n", arguments);
            std::cout<<"(arguments & 0x0000ff00): "<<(arguments & 0x0000ff00)<<std::endl;

            instructionsVector[i].arguments[3] = (uint8_t)((arguments & 0xff000000) >> 24);
            instructionsVector[i].arguments[2] = (uint8_t)((arguments & 0x00ff0000) >> 16);
            instructionsVector[i].arguments[1] = (uint8_t)((arguments & 0x0000ff00) >> 8);
            instructionsVector[i].arguments[0] = (uint8_t)(arguments & 0x000000ff);

            std::cout<<"instruction.arguments[2]: "<<+instructionsVector[i].arguments[2]<<std::endl;
            std::cout<<"instruction.arguments[3]: "<<+instructionsVector[i].arguments[3]<<std::endl;
        }
    }

    for(unsigned int i = 0; i < instructionsVector.size(); ++i)
    {
        std::cout<<"i: "<<i<<std::endl;
        std::cout<<"Instruction: "<<instructionsVector[i].instructionString<<std::endl;
        std::cout<<"Arguments: "<<instructionsVector[i].argumentsSize<<std::endl;
    }

    outFileBufSize = getInstructionAddress(instructionsVector);

    outFileBuf = new char[getInstructionAddress(instructionsVector)];

    std::cout<<"Building outFileBuf of size: "<<outFileBufSize<<std::endl;

    unsigned int outFileBufIndex = 0;
    for(unsigned int i = 0; i < instructionsVector.size(); ++i)
    {
        std::cout<<"Processing instruction: "<<+instructionsVector[i].instruction<<std::endl;
        outFileBuf[outFileBufIndex] = instructionsVector[i].instruction;
        outFileBufIndex++;
        if(instructionsVector[i].instructionSize == 1)
        {

        }
        else
        {
            for(unsigned int n = 0; n < instructionsVector[i].argumentsSize; ++n)
            {

                outFileBuf[outFileBufIndex] = instructionsVector[i].arguments[n];
                outFileBufIndex++;
            }
        }
        std::cout<<"outFileBufIndex: "<<outFileBufIndex<<std::endl;
    }
}
