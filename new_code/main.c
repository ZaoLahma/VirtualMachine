#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./FileReader.h"

#include "./VM_CPU.h"

int main(int argc, char **argv)
{
    printf("Hello world!\n");

    unsigned int debug = 0;

    if(argc == 3)
    {
        printf("argv[2]: %s\n", argv[2]);

        if(strcmp(argv[2], "debug") == 0)
        {
            printf("DEBUG\n");
            debug = 1;
        }
    }

    struct CPU cpu;

    readFile(argv[1], &cpu.RAM[0]);

    init_cpu(&cpu);

   // disassemble_program(&cpu);

    //reset_cpu(&cpu);

    execute_program(&cpu, debug);

    return 0;
}
