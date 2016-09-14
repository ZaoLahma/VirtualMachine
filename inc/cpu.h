#ifndef HEADER_VM_CPU
#define HEADER_VM_CPU

#include <stdint.h>

#define NO_OF_REGISTERS 5


struct vm_cpuS
{
	uint32_t registers[NO_OF_REGISTERS];
	
	uint32_t stack[255];
	
	uint8_t sp;
	
	uint32_t pc;
};


void initialize_vm_cpuS(struct vm_cpuS* cpuPtr);
void execute_program(struct vm_cpuS* cpuPtr, const char* program);

#endif
