#include "../inc/VM_CPU.h"
#include "../inc/InstructionSet.h"

#include <stdint.h>
#include <stdio.h>

void init_cpu(struct CPU* cpuPtr)
{
    cpuPtr->pc = cpuPtr->RAM;
    cpuPtr->sp = 256; //One above stack limit
}

void reset_cpu(struct CPU* cpuPtr)
{
    init_cpu(cpuPtr);
}

void disassemble_program(struct CPU* cpuPtr)
{
    unsigned int counter = 0;
    unsigned int stop = 0;
    int8_t high;
    int8_t low;
    int16_t val;
    while(0 == stop)
    {
        printf("Instruction: 0x%x\n", *(cpuPtr->pc));
        switch(*(cpuPtr->pc))
        {
            case NOP:
                printf("NOP\n");
                break;
            case PUSH:
                printf("PUSH 0x%x\n", *(cpuPtr->pc + 1));
                cpuPtr->pc+=1;
                break;
            case POP:
                printf("POP\n");
                break;
            case LOAD:
                printf("LOAD ");
                printf("%u\n", *(cpuPtr->pc + 1));
                cpuPtr->pc+=1;
                break;
            case STORE:
                printf("STORE ");
                printf("%u\n", *(cpuPtr->pc + 1));
                cpuPtr->pc+=1;
                break;
            case JMP:
                printf("JMP ");
                low = *(cpuPtr->pc + 1);
                high = *(cpuPtr->pc + 2);
                printf("high: 0x%x, low: 0x%x\n", high, low);
                val = ((high << 8) | low);
                printf("%d\n", val);
                cpuPtr->pc+=2;
                break;
            case JZ:
                printf("JZ ");
                low = *(cpuPtr->pc + 1);
                high = *(cpuPtr->pc + 2);
                printf("high: 0x%x, low: 0x%x\n", high, low);
                val = ((high << 8) | low);
                printf("%d\n", val);
                cpuPtr->pc+=2;
                break;
            case JNZ:
                printf("JNZ ");
                low = *(cpuPtr->pc + 1);
                high = *(cpuPtr->pc + 2);
                printf("high: 0x%x, low: 0x%x\n", high, low);
                val = ((high << 8) | low);
                printf("%d\n", val);
                cpuPtr->pc+=2;
                break;
            case ADD:
                printf("ADD\n");
                break;
            case SUB:
                printf("SUB\n");
                break;
            case MUL:
                printf("MUL\n");
                break;
            case DIV:
                printf("DIV\n");
                break;
            case PRINT:
                printf("PRINT\n");
                break;
            case STOP:
                printf("STOP\n");
                stop = 1;
                break;
            default:
                printf("Instruction: 0x%x not implemented\n", *(cpuPtr->pc));
            break;
        }
        cpuPtr->pc++;
        counter++;
    }
    printf("counter: %d\n", counter);
}

void popStack(struct CPU* cpuPtr)
{
    if(cpuPtr->sp < 256)
    {
        cpuPtr->sp+=1;
    }
    else
    {
        printf("ERROR: Stack underflow. Crashing.");
        char* crash = 0;
        *crash = 5;
    }
}

void pushStack(struct CPU* cpuPtr)
{
    if(cpuPtr->sp > 0)
    {
        cpuPtr->sp-=1;
    }
    else
    {
        printf("ERROR: Stack overflow. Crashing.");
        char* crash = 0;
        *crash = 5;
    }
}

void execute_program(struct CPU* cpuPtr, unsigned int debug)
{
    printf("EXECUTION START\n");
    unsigned int counter = 0;
    unsigned int stop = 0;
    uint16_t high_16;
    uint16_t low_16;
    uint8_t high;
    uint8_t low;
    unsigned int val_32;
    int16_t val;
    while(0 == stop)
    {
        if(debug)
        {
            printf("cpuPtr->sp: %u\n", cpuPtr->sp);
            printf("Instruction: 0x%x\n", *(cpuPtr->pc));
        }
        switch(*(cpuPtr->pc))
        {
            case NOP:
                cpuPtr->pc+=1;
                break;
            case PUSH:
                low_16 = *((uint16_t*)(cpuPtr->pc + 1));
                high_16 = *((uint16_t*)(cpuPtr->pc + 3));
                val_32 = (high_16 << 16);
                val_32 = val_32 | low_16;
                if(debug)
                {
                    printf("PUSH 0x%x\n", val_32);
                }
                pushStack(cpuPtr);
                cpuPtr->stackMem[cpuPtr->sp] = val_32;
                cpuPtr->pc+=5;
                break;
            case POP:
                if(debug)
                {
                    printf("POP\n");
                }
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case LOAD:
                if(debug)
                {
                    printf("LOAD \n");
                }
                pushStack(cpuPtr);
                cpuPtr->stackMem[cpuPtr->sp] = cpuPtr->registers[(int)*(cpuPtr->pc + 1)];
                cpuPtr->pc+=2;
                break;
            case STORE:
                if(debug)
                {
                    printf("STORE ");
                    printf("%u: %u\n", *(cpuPtr->pc + 1), cpuPtr->stackMem[cpuPtr->sp]);
                }
                cpuPtr->registers[(int)*(cpuPtr->pc + 1)] = cpuPtr->stackMem[cpuPtr->sp];
                popStack(cpuPtr);
                cpuPtr->pc+=2;
                break;
            case JMP:
                if(debug)
                {
                    printf("JMP ");
                }
                low = *(cpuPtr->pc + 1);
                high = *(cpuPtr->pc + 2);
                if(debug)
                {
                    printf("high: 0x%x, low: 0x%x\n", high, low);
                }
                val = ((high << 8) | low);
                if(debug)
                {
                    printf("%d\n", val);
                }
                cpuPtr->pc = &cpuPtr->RAM[val];
                break;
            case JZ:
                if(debug)
                {
                    printf("JZ %d\n", cpuPtr->stackMem[cpuPtr->sp]);
                }
                if(0 == (cpuPtr->stackMem[cpuPtr->sp]))
                {
                    popStack(cpuPtr);
                    low = *(cpuPtr->pc + 1);
                    high = *(cpuPtr->pc + 2);
                    if(debug)
                    {
                        printf("high: 0x%x, low: 0x%x\n", high, low);
                    }
                    val = ((high << 8) | low);
                    if(debug)
                    {
                        printf("%d\n", val);
                    }
                    cpuPtr->pc = &cpuPtr->RAM[val];
                }
                else
                {
                    popStack(cpuPtr);
                    cpuPtr->pc+=3;
                }
                break;
            case JNZ:
                if(debug)
                {
                    printf("JNZ ");
                }
                if(0 != (cpuPtr->stackMem[cpuPtr->sp]))
                {
                    popStack(cpuPtr);;
                    low = *(cpuPtr->pc + 1);
                    high = *(cpuPtr->pc + 2);
                    if(debug)
                    {
                        printf("high: 0x%x, low: 0x%x\n", high, low);
                    }
                    val = ((high << 8) | low);
                    if(debug)
                    {
                        printf("%d\n", val);
                    }
                    cpuPtr->pc = &cpuPtr->RAM[val];
                }
                else
                {
                    popStack(cpuPtr);
                    cpuPtr->pc+=3;
                }
                break;
            case ADD:
                if(debug)
                {
                    printf("ADD\n");
                }
                cpuPtr->stackMem[cpuPtr->sp + 1] = (cpuPtr->stackMem[cpuPtr->sp + 1] + cpuPtr->stackMem[cpuPtr->sp]);
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case SUB:
                if(debug)
                {
                    printf("SUB %d - %d\n", cpuPtr->stackMem[cpuPtr->sp + 1], cpuPtr->stackMem[cpuPtr->sp]);
                }
                cpuPtr->stackMem[cpuPtr->sp + 1] = (cpuPtr->stackMem[cpuPtr->sp + 1] - cpuPtr->stackMem[cpuPtr->sp]);
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case MUL:
                if(debug)
                {
                    printf("MUL\n");
                }
                cpuPtr->stackMem[cpuPtr->sp + 1] = (cpuPtr->stackMem[cpuPtr->sp + 1] * cpuPtr->stackMem[cpuPtr->sp]);
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case DIV:
                if(debug)
                {
                    printf("DIV\n");
                }
                cpuPtr->stackMem[cpuPtr->sp + 1] = (cpuPtr->stackMem[cpuPtr->sp + 1] / cpuPtr->stackMem[cpuPtr->sp]);
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case PRINT:
                if(debug)
                {
                    printf("PRINT\n");
                }
                printf("TOS: %d\n", cpuPtr->stackMem[cpuPtr->sp]);
                popStack(cpuPtr);
                cpuPtr->pc+=1;
                break;
            case STOP:
                if(debug)
                {
                    printf("STOP\n");
                }
                stop = 1;
                break;
            case CALL:
                if(debug)
                {
                    printf("CALL\n");
                }
                cpuPtr->pc+=3;
                break;
            default:
                printf("Instruction: 0x%x not implemented\n", *(cpuPtr->pc));
            break;
        }
        counter++;
    }
    printf("EXECUTION END\n");
    printf("Instructions processed: %d\n", counter);
}
