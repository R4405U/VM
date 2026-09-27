#include "cpu.h"
#include "memory.h"
#include "registers.h"
#include "defines.h"
#include "opcode.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>





void cpu_init(cpu_state* cpu){
    cpu_reset(cpu);
}

void cpu_reset(cpu_state* cpu){
    memset(cpu->regs, 0, sizeof(cpu->regs));

    cpu->regs[Z_REG]  = 0;
    cpu->regs[PC_REG] = BOOTLOADER_START;
    cpu->regs[SP_REG] = STACK_INIT_TOP;
    cpu->regs[FP_REG] = STACK_INIT_TOP;

    cpu->is_kernel_mode = true;
}


word_t cpu_fetch(cpu_state* cpu,  mem_bus* bus){
    cpu->regs[Z_REG] = 0;

    word_t instr = mem_read32(bus, cpu->regs[PC_REG]);

    if(bus->bus_err){
        fprintf(stderr, "[CPU TRAP] Bus Error during fetch at 0x%08X\n", cpu->regs[PC_REG]);
        cpu->regs[FG_REG] |= FLAG_HALT;
        return 0;
    }

    cpu->regs[PC_REG] += 4;
    return instr;
}

void cpu_step(cpu_state* cpu, mem_bus* bus){
    if(cpu->regs[FG_REG] & FLAG_HALT){
        return;
    }

    word_t instr = cpu_fetch(cpu, bus);
    if(cpu->regs[FG_REG] & FLAG_HALT){
        return;
    }

    byte_t opcode = GET_OPCODE(instr);
    byte_t rd = GET_RD(instr);
    byte_t rs1 = GET_R1(instr);
    byte_t rs2 = GET_R2(instr);
    uint16_t imm16 = GET_IMM16(instr);


    switch (opcode) {
        case OP_NOP:
            break;

        case OP_ADD:
            cpu->regs[rd] = cpu->regs[rs1] + cpu->regs[rs2];
            break;

        case OP_SUB:
            cpu->regs[rd] = cpu->regs[rs1] - cpu->regs[rs2];

            if (cpu->regs[rd] == 0) cpu->regs[FG_REG] |= FLAG_ZERO;
            else                    cpu->regs[FG_REG] &= ~FLAG_ZERO;

            if (cpu->regs[rd] & 0x80000000) cpu->regs[FG_REG] |= FLAG_NEGATIVE;
            else                            cpu->regs[FG_REG] &= ~FLAG_NEGATIVE;
            break;

        case OP_MUL: {

            uint64_t val1 = (uint64_t)cpu->regs[rs1];
            uint64_t val2 = (uint64_t)cpu->regs[rs2];

            uint64_t result = val1 * val2;

            cpu->regs[rd] = (word_t)(result & 0xFFFFFFFF);
            cpu->regs[T1_REG] = (word_t)(result >> 32);

            if(cpu->regs[rd] == 0)  cpu->regs[FG_REG] |= FLAG_ZERO;
            else                    cpu->regs[FG_REG] &= ~FLAG_ZERO;

            if(cpu->regs[rd] & 0x80000000)  cpu->regs[FG_REG] |= FLAG_NEGATIVE;
            else                            cpu->regs[FG_REG] &= ~FLAG_NEGATIVE;

            break;
        }


        case OP_DIV: {
            word_t divisor = cpu->regs[rs2];

            if(divisor == 0){
                fprintf(stderr, "[CPU TRAP] Division by zero at PC: 0x%08X\n",
                        cpu->regs[PC_REG] - 4);
                cpu->regs[FG_REG] |= FLAG_HALT; // Halt execution loop
                break;
            }

            word_t dividend = cpu->regs[rs1];


            cpu->regs[rd] = dividend / divisor;
            cpu->regs[T1_REG] = dividend % divisor;

            if (cpu->regs[rd] == 0) cpu->regs[FG_REG] |= FLAG_ZERO;
            else                    cpu->regs[FG_REG] &= ~FLAG_ZERO;
            break;
        }

        case OP_MOD: {
            word_t divisor = cpu->regs[rs2];

            if (divisor == 0) {
                fprintf(stderr, "[CPU TRAP] Modulo by zero at PC: 0x%08X\n",
                        cpu->regs[PC_REG] - 4);
                cpu->regs[FG_REG] |= FLAG_HALT;
                break;
            }

            cpu->regs[rd] = cpu->regs[rs1] % divisor;
            break;
        }



        case OP_AND:
            cpu->regs[rd] = cpu->regs[rs1] & cpu->regs[rs2];
            break;



        case OP_OR:
            cpu->regs[rd] = cpu->regs[rs1] | cpu->regs[rs2];
            break;


        case OP_XOR:
            cpu->regs[rd] = cpu->regs[rs1] ^ cpu->regs[rs2];
            break;


        case OP_ADDI:
            cpu->regs[rd] = cpu->regs[rs1] + (word_t)imm16;
            break;


        case OP_JMP:
            cpu->regs[PC_REG] = (word_t)GET_JUMP26(instr);
            break;

        case OP_JZ: {
            if(cpu->regs[FG_REG] & FLAG_ZERO){
                cpu->regs[PC_REG] = (word_t)GET_JUMP26(instr);
            }
            break;
        }

        case OP_JNZ: {
            if(!(cpu->regs[FG_REG] & FLAG_ZERO)){
                cpu->regs[PC_REG] = (word_t)GET_JUMP26(instr);
            }
            break;
        }


        case OP_PUSH:
            cpu->regs[SP_REG] -= 4;
            mem_write32(bus, cpu->regs[SP_REG], cpu->regs[rd]);
            break;

        case OP_POP:

            cpu->regs[rd] = mem_read32(bus, cpu->regs[SP_REG]);
            cpu->regs[SP_REG] += 4;
            break;


        case OP_HALT:
            cpu->regs[FG_REG] |= FLAG_HALT;
            break;

        case OP_CALL: {

            word_t target_addr = (word_t)GET_JUMP26(instr);

            cpu->regs[SP_REG] -= 4;

            mem_write32(bus, cpu->regs[SP_REG], cpu->regs[PC_REG]);

            if(cpu->regs[FG_REG] & FLAG_HALT){
                fprintf(stderr, "[CPU TRAP] Stack Overflow or Bus Error at CALL\n");
                break;
            }

            cpu->regs[PC_REG] = target_addr;
            break;
        }

        case OP_RET: {
            word_t return_addr = mem_read32(bus, cpu->regs[SP_REG]);

            if (cpu->regs[FG_REG] & FLAG_HALT) {
                fprintf(stderr, "[CPU TRAP] Stack Underflow or Bus Error at RET\n");
                break;
            }

            cpu->regs[SP_REG] += 4;

            cpu->regs[PC_REG] = return_addr;
            break;
        }


        default:
            fprintf(stderr, "[CPU EXCEPTION] Illegal Opcode 0x%02X at PC 0x%08X\n",
                    opcode, cpu->regs[PC_REG] - 4);
            cpu->regs[FG_REG] |= FLAG_HALT;
            break;

    }

    cpu->regs[Z_REG] = 0;

}

void cpu_run(cpu_state* cpu, mem_bus* bus){
    while(!(cpu->regs[FG_REG] & FLAG_HALT)){
        cpu_step(cpu, bus);
    }
}

void cpu_dump_regs(const cpu_state *cpu) {
    printf("=== CPU REGISTERS DUMP ===\n");
    for (int i = REG_R0; i <= REG_R12; i++) {
        printf("R%-2d: 0x%08X  ", i, cpu->regs[i]);
        if ((i + 1) % 4 == 0) printf("\n");
    }
    printf("\n--- NAMED / SPECIAL REGISTERS ---\n");
    printf("Z_REG : 0x%08X | RA_REG: 0x%08X | SP_REG : 0x%08X\n", cpu->regs[Z_REG], cpu->regs[RA_REG], cpu->regs[SP_REG]);
    printf("GP_REG: 0x%08X | TP_REG: 0x%08X | PC_REG : 0x%08X\n", cpu->regs[GP_REG], cpu->regs[TP_REG], cpu->regs[PC_REG]);
    printf("FP_REG: 0x%08X | FG_REG: 0x%08X | PFG_REG: 0x%08X\n", cpu->regs[FP_REG], cpu->regs[FG_REG], cpu->regs[PFG_REG]);
    printf("T1_REG: 0x%08X\n", cpu->regs[T1_REG]);
    printf("FLAGS : [ %s%s%s%s%s]\n",
           (cpu->regs[FG_REG] & FLAG_ZERO)     ? "Z " : "",
           (cpu->regs[FG_REG] & FLAG_NEGATIVE) ? "N " : "",
           (cpu->regs[FG_REG] & FLAG_CARRY)    ? "C " : "",
           (cpu->regs[FG_REG] & FLAG_OVERFLOW) ? "V " : "",
           (cpu->regs[FG_REG] & FLAG_HALT)     ? "HALTED " : "RUNNING ");
    printf("===========================\n\n");
}
