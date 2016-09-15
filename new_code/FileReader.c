#include "./FileReader.h"
#include <stdio.h>

unsigned int readFile(const char* fileName, char* buf)
{
    unsigned int numbytes = 0;

    FILE * pFile;

    pFile = fopen (fileName, "r");
    if (pFile == NULL)
    {
        printf("Error opening file");
    }
    else
    {
        fseek(pFile, 0L, SEEK_END);
        numbytes = ftell(pFile);

        fseek(pFile, 0L, SEEK_SET);

        fread(buf, sizeof(char), numbytes, pFile);

        printf("Loaded %d bytes into buf at address: %p\n", numbytes, buf);

        fclose (pFile);
    }

    return numbytes;
}

unsigned int getFileSize(const char* fileName)
{
    unsigned int numbytes = 0;

    FILE * pFile;

    pFile = fopen (fileName, "r");
    if (pFile == NULL)
    {
        printf("Error opening file");
    }
    else
    {
        fseek(pFile, 0L, SEEK_END);
        numbytes = ftell(pFile);

        fseek(pFile, 0L, SEEK_SET);

        fclose(pFile);
    }

    return numbytes;
}

void writeFile(const char* fileName, const char* buf, const unsigned int numbytes)
{
    FILE * pFile;

    pFile = fopen (fileName, "w+");
    if (pFile == NULL)
    {
        printf("Error opening file");
    }
    else
    {
        fwrite(buf, sizeof(char), numbytes, pFile);

        fclose (pFile);
    }
}
