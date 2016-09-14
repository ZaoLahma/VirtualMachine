#include "./cpu.h"
#include <string.h>
#include <stdio.h>
#include "./instr_set.h"


void print_instruction(const uint8_t instruction, const vm_cpuS* cpuPtr, const char* program);
void abort_exec(const char* str);
void pop_stack(vm_cpuS* cpuPtr);
void push_stack(vm_cpuS* cpuPtr);

void initialize_vm_cpuS(vm_cpuS* cpu)
{
	printf("VM booting. Initializing CPU\n");
	
	memset(&cpu->registers, 0x0, sizeof(cpu->registers));
	
	cpu->sp = 255;
	
	cpu->pc = 4;
}


void execute_program(vm_cpuS* cpuPtr, const char* program)
{
	printf("VM executing program\n");
	
	bool executeFinished = false;
	
	char expectedBytePattern[] = {0xff, 0xfe, 0xf1, 0xf0};
	
	if(0 != memcmp(program, &expectedBytePattern, 4))
	{
		abort_exec("Not a valid format executable\n");
	}
	else
	{
		printf("Executable format validated OK\n");
	}
	
	uint8_t instruction;
	while(!executeFinished)
	{
		instruction = program[cpuPtr->pc];
		switch(instruction)
		{
			case NOP:
				cpuPtr->pc += 1;
				break;
			
			case PUSH:
			{
				push_stack(cpuPtr);
				uint32_t valToSet = 0;
				valToSet = ((uint8_t)program[cpuPtr->pc + 1] << 24) | 
				           ((uint8_t)program[cpuPtr->pc + 2] << 16) | 
				           ((uint8_t)program[cpuPtr->pc + 3] << 8) | 
				           ((uint8_t)program[cpuPtr->pc + 4]);
				cpuPtr->stack[cpuPtr->sp] = valToSet;
				cpuPtr->pc += 5;
			}
			break;
				
			case POP:
				pop_stack(cpuPtr);
				cpuPtr->pc += 1;
				break;
			
			case STORE:
			{
				uint8_t registerIndex = (uint8_t)program[cpuPtr->pc + 1];
				cpuPtr->registers[registerIndex] = cpuPtr->stack[cpuPtr->sp];
				pop_stack(cpuPtr);
				cpuPtr->pc += 2;
			}
				break;
				
			case LOAD:
				push_stack(cpuPtr);
				cpuPtr->stack[cpuPtr->sp] = cpuPtr->registers[(uint8_t)program[cpuPtr->pc + 1]];
				cpuPtr->pc += 2;
				break;
			
			case ADD:
			{
				uint32_t res = cpuPtr->stack[cpuPtr->sp + 1] + cpuPtr->stack[cpuPtr->sp];
				pop_stack(cpuPtr);
				cpuPtr->stack[cpuPtr->sp] = res;
				cpuPtr->pc += 1;
			}
			break;

			case SUB:
			{
				uint32_t res = cpuPtr->stack[cpuPtr->sp + 1] - cpuPtr->stack[cpuPtr->sp];
				pop_stack(cpuPtr);
				cpuPtr->stack[cpuPtr->sp] = res;
				cpuPtr->pc += 1;
			}
			break;
			
			case MUL:
			{
				uint32_t res = cpuPtr->stack[cpuPtr->sp + 1] * cpuPtr->stack[cpuPtr->sp];
				pop_stack(cpuPtr);
				cpuPtr->stack[cpuPtr->sp] = res;
				cpuPtr->pc += 1;
			}
			break;
			
			case DIV:
			{
				uint32_t res = cpuPtr->stack[cpuPtr->sp + 1] / cpuPtr->stack[cpuPtr->sp];
				pop_stack(cpuPtr);
				cpuPtr->stack[cpuPtr->sp] = res;
				cpuPtr->pc += 1;
			}
			break;
			
			case PRINT:
				printf("%d\n", cpuPtr->stack[cpuPtr->sp]);
				cpuPtr->pc += 1;
			break;
			
			case END_EXEC:
				executeFinished = true;
			break;
				
			default:
				break;
		}
		print_instruction(instruction, cpuPtr, program);
	}
}

void pop_stack(vm_cpuS* cpuPtr)
{
	if((uint8_t)cpuPtr->sp < (uint8_t)255)
	{
		cpuPtr->sp += 1;
	}
	else
	{
		abort_exec("Incrementing stack pointer outside of stack (sp > 254)");
	}
}

void push_stack(vm_cpuS* cpuPtr)
{
	if(cpuPtr->sp > 0)
	{
		cpuPtr->sp -= 1;
	}
	else
	{
		abort_exec("Incrementing stack pointer outside of stack (sp < 0)");
	}
}

void abort_exec(const char* str)
{
	printf("Aborting execution: %s.", str);
	
	char* crashPoint = NULL;
	*crashPoint = 0;
}

void print_instruction(const uint8_t instruction, const vm_cpuS* cpuPtr, const char* program)
{
	switch(instruction)
	{
		case NOP:
			printf("NOP\n");
			break;

			case PUSH:
				printf("PUSH cpuPtr->stack[%d]: 0x%x\n", 
				       cpuPtr->sp, 
				       cpuPtr->stack[cpuPtr->sp]);
				break;
			
			case POP:
				printf("POP cpuPtr->sp: %d\n", cpuPtr->sp);
				break;
			
			case END_EXEC:
				printf("END_EXEC\n");
				break;
				
			case STORE:
			{
				uint8_t registerIndex = (uint8_t)program[cpuPtr->pc - 1];
				printf("STORE: registerIndex: %d, cpuPtr->stack[%d]: 0x%x\n", 
						registerIndex,
						cpuPtr->sp - 1,
						cpuPtr->stack[cpuPtr->sp - 1]);
			}
			break;
			
			case LOAD:
			{
				uint8_t registerIndex = (uint8_t)program[cpuPtr->pc - 1];
				printf("LOAD: registerIndex: %d, cpuPtr->stack[%d]: 0x%x\n",
						registerIndex,
						cpuPtr->sp,
						cpuPtr->stack[cpuPtr->sp]);
			}
			break;
			
			case ADD:
			{
				printf("ADD: cpuPtr->stack[%d]: 0x%x\n",
						cpuPtr->sp,
						cpuPtr->stack[cpuPtr->sp]);
			}
			break;
			
			case SUB:
			{
				printf("SUB: cpuPtr->stack[%d]: 0x%x\n",
						cpuPtr->sp,
						cpuPtr->stack[cpuPtr->sp]);
			}
			break;
			
			case MUL:
			{
				printf("MUL: cpuPtr->stack[%d]: 0x%x\n",
						cpuPtr->sp,
						cpuPtr->stack[cpuPtr->sp]);
			}
			break;
			
			case DIV:
			{
				printf("DIV: cpuPtr->stack[%d]: 0x%x\n",
						cpuPtr->sp,
						cpuPtr->stack[cpuPtr->sp]);
			}
			break;
			
			case PRINT:
				printf("PRINT\n");
			break;
		
		default:
			printf("Instruction: 0x%X not implemented\n", instruction);
			break;
	}
}
