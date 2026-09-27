

#ifndef OPCODE_H
#define OPCODE_H

typedef enum {
    OP_ADD  = 0x01,
    OP_SUB  = 0x02,
    OP_MUL  = 0x03,
    OP_DIV  = 0x04,
    OP_MOD  = 0x05,
    OP_AND  = 0x06,
    OP_OR   = 0x07,
    OP_XOR  = 0x08,

    OP_NOP  = 0x9,

    OP_ADDI = 0x10,
    OP_LOAD = 0x11,
    OP_STR  = 0x12, // STORE
    OP_JZ   = 0x13,
    OP_JNZ  = 0x14,

    OP_JMP  = 0x20,
    OP_CALL = 0x21,
    OP_RET  = 0x22,

    OP_PUSH = 0x30,
    OP_POP  = 0x31,
    OP_HALT = 0x3F
} opcode_t;


#endif // OPCODE_H
