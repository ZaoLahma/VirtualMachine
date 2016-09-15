#ifndef FILEREADER_HEADER
#define FILEREADER_HEADER

unsigned int readFile(const char* fileName, char* buf);

unsigned int getFileSize(const char* fileName);

void writeFile(const char* fileName, const char* buf, const unsigned int numbytes);

#endif
