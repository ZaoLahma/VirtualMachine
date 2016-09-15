#ifndef VM_CPU_H_INCLUDED
#define VM_CPU_H_INCLUDED
#include <stdint.h>
struct CPU
{
    unsigned int registers[16];

    unsigned int stackMem[256];

    unsigned int sp;
    unsigned char* pc;

    char RAM[65536];
};

void init_cpu(struct CPU* cpuPtr);

void reset_cpu(struct CPU* cpuPtr);

void disassemble_program(struct CPU* cpuPtr);

void execute_program(struct CPU* cpuPtr, unsigned int debug);

#endif // VM_CPU_H_INCLUDED
