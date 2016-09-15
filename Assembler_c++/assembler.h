#include <string>
#include <stdint.h>

class Instruction
{
    public:
    std::string label;
    char instruction;
    std::string argumentsString;
    uint8_t arguments[4];
    unsigned int argumentsSize;
    unsigned int instructionSize;
    unsigned int address;
    std::string instructionString;

    Instruction():
    label(""),
    instructionString("")
    {

    }
};


class Assembler
{
    private:
    Assembler();

    const char* fileBufPtr;
    const unsigned int fileSize;
    char* outFileBuf;
    unsigned int outFileBufSize;

    std::string& trim_right(std::string& s, const std::string& delimiters = " \f\n\r\t\v");
    std::string& trim_left(std::string& s, const std::string& delimiters = " \f\n\r\t\v");
    std::string& trim(std::string& s, const std::string& delimiters = " \f\n\r\t\v");

    void tryGetLabel(const std::string str, Instruction& instruction);
    void tryGetInstruction(const std::string str, Instruction& instruction);
    void tryGetArgument(const std::string str, Instruction& instruction);

    public:
    Assembler(const char* fileBuf, const unsigned int fileSize);

    void assemble();

    const char* getOutFileBuf()
    {
        return outFileBuf;
    }

    const unsigned int getOutFileBufSize()
    {
        return outFileBufSize;
    }

};
