#ifndef REGISTERS_H
#define REGISTERS_H


// #define TOTAL_REGS 23;



typedef enum {
    REG_R0 = 0, REG_R1, REG_R2, REG_R3,
    REG_R4,     REG_R5, REG_R6, REG_R7,
    REG_R8,     REG_R9, REG_R10, REG_R11,
    REG_R12,

} register_indx;

typedef enum {
    Z_REG = 13,
    RA_REG,
    SP_REG,
    GP_REG,
    TP_REG,
    PC_REG,
    FP_REG,
    FG_REG,
    PFG_REG,
    T1_REG

} named_registers;



#endif // REGISTERS_H
