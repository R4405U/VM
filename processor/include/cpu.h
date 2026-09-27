#include "vm_types.h"
#include "memory.h"
#include "defines.h"
#include <stdbool.h>

#ifndef CPU_H
#define CPU_H


#define GET_OPCODE(i)   (((i) >> 26) & 0x3F)                // Bit [31:26] - 6 bits
#define GET_RD(i)       (((i) >> 21) & 0x1F)                // Bit [25:21] - 5 bits
#define GET_R1(i)       (((i) >> 16) & 0x1F)                // Bit [20:16] - 5 bits
#define GET_R2(i)       (((i) >> 11) & 0x1F)                // Bit [15:11] - 5 bits
#define GET_IMM16(i)    ((uint16_t)((i) & 0xFFFF))          // Bit [15:0]  - 16 bits signed
#define GET_JUMP26(i)   ((i) & 0x03FFFFFF)                  // Bit [25:0]  - 26 bits


typedef enum {
    FLAG_ZERO     = (1 << 0),
    FLAG_NEGATIVE = (1 << 1),
    FLAG_CARRY    = (1 << 2),
    FLAG_OVERFLOW = (1 << 3),
    FLAG_HALT     = (1 << 4)
}cpu_flags;

typedef struct {
    word_t regs[TOTAL_REGS];
    bool is_kernel_mode;
}cpu_state;


void cpu_init(cpu_state* cpu);
void cpu_reset(cpu_state* cpu);
word_t cpu_fetch(cpu_state* cpu, mem_bus* bus);
void cpu_step(cpu_state* cpu, mem_bus* bus);
void cpu_run(cpu_state* cpu, mem_bus* bus);
void cpu_dump_regs(const cpu_state* cpu);


#endif // CPU_H
