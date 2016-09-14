#include "../inc/cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include "../inc/instr_set.h"

int main(void)
{	
	struct vm_cpuS* cpuPtr = (struct vm_cpuS*) malloc(sizeof(struct vm_cpuS));
	
	initialize_vm_cpuS(cpuPtr);
	
	char program[] = {0xff, 0xfe, 0xf1, 0xf0,
					  PUSH, 0xf1, 0xef, 0x1, 0x2,
					  STORE, 0x0,
					  PUSH, 0xff, 0xff, 0xff, 0xff,
					  POP,
					  LOAD, 0x0,
					  POP,
					  PUSH, 0x0, 0x0, 0x0, 0x1,
					  PUSH, 0x0, 0x0, 0x0, 0x2,
					  ADD,
					  PRINT,
					  POP,
					  PUSH, 0x0, 0x0, 0x0, 0x3,
					  PUSH, 0x0, 0x0, 0x0, 0x2,					  
					  SUB,
					  PRINT,
					  POP,
					  PUSH, 0x0, 0x0, 0x0, 0x4,
					  PUSH, 0x0, 0x0, 0x0, 0x2,					  
					  MUL,
					  PRINT,
					  POP,
					  PUSH, 0x0, 0x0, 0x0, 0x4,
					  PUSH, 0x0, 0x0, 0x0, 0x2,					  
					  DIV,
					  PRINT,					  					  
					  END_EXEC
					  };
	
	execute_program(cpuPtr, program);
	
	free(cpuPtr);
	
	return 0;
}
