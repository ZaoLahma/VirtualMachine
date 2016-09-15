#include <iostream>

#include "./assembler.h"
#include "../inc/FileReader.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    unsigned int fileSize = getFileSize(argv[1]);

    std::cout<<"fileSize: "<<fileSize<<std::endl;

    char* fileBuf = new char[fileSize];

    readFile(argv[1], fileBuf);

    Assembler assembler(fileBuf, fileSize);

    assembler.assemble();

    const unsigned int outFileBufSize = assembler.getOutFileBufSize();
    const char* outFileBuf = assembler.getOutFileBuf();

    std::string outFileName = argv[1];

    outFileName = outFileName.substr(0, outFileName.find_last_of(".") + 1);

    outFileName+= "bcm";

    std::cout<<"outFileName: "<<outFileName<<std::endl;

    writeFile(outFileName.c_str(), outFileBuf, outFileBufSize);

    delete[] fileBuf;

    delete outFileBuf;

    return 0;
}
