# ISA

## Register to Register Operations
    *Used for Operations Like ADD, SUB, AND, OR, XOR, ...*
    
   1. OpCode (Operation Code):      Unique Numerical Identifier Which Tells Which Operation To Perform.
   2. Rd (Destination Register):    The Register Where The Result Of Operation Will Be Written.
   3. Rs1, Rs2 (Source Register):   The Registers Holding Input Operands For Operation.
   4. Imm (Immediate Value):        A Constant Number Encoded Directly Into Instruction.
   
   | Field | Opcode | $R_d$ (Destination) | $R_{s1}$ (Source 1) | $R_{s2}$ (Source 2) | Unused |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Bit Range** | `[31:26]` | `[25:21]` | `[20:16]` | `[15:11]` | `[10:0]` |
| **Bit Width** | 6 bits | 5 bits | 5 bits | 5 bits | 11 bits |

## 2. I-Type (Immediate & Memory Operations)
    *Used for immediate math, loads, stores, and conditional branches: `ADDI`, `LOAD`, `STORE`, `JZ`, `JNZ`.*

| Field | Opcode | $R_d$ (Destination / Src) | $R_{s1}$ (Base Register) | Immediate Constant ($Imm_{16}$) |
| :--- | :---: | :---: | :---: | :---: |
| **Bit Range** | `[31:26]` | `[25:21]` | `[20:16]` | `[15:0]` |
| **Bit Width** | 6 bits | 5 bits | 5 bits | 16 bits |

## 3. J-Type (Unconditional Jump & Call Operations)
    *Used for long-distance control flow jumps and function calls: `JMP`, `CALL`.*

| Field | Opcode | Absolute Target Address |
| :--- | :---: | :---: |
| **Bit Range** | `[31:26]` | `[25:0]` |
| **Bit Width** | 6 bits | 26 bits |

## 4. S-Type (Stack Operations)
    *Used for single-register push/pop stack instructions: `PUSH`, `POP`.*

| Field | Opcode | Target Register ($Reg$) | Unused |
| :--- | :---: | :---: | :---: |
| **Bit Range** | `[31:26]` | `[25:21]` | `[20:0]` |
| **Bit Width** | 6 bits | 5 bits | 21 bits |



# Memory Layout

Address Range            Size       Region Name          Usage / Behavior
---------------------------------------------------------------------------------------
0x00000000 - 0x000001FF  512 B      1. Bootloader        Reset Vector & initial boot code
0x00000200 - 0x000CFFFF  831.5 KB   2. General RAM       User Code, Static Data, Heap (Grows UP ↑)
0x000D0000 - 0x000DFFFF  64 KB      3. Stack Region      Stack Memory (Grows DOWN ↓ from 0x000DFFFF)
0x000E0000 - 0x000EFFFF  64 KB      4. Video Memory      Frame buffer (Text Mode / Framebuffer)
0x000F0000 - 0x000FFFFF  64 KB      5. MMIO Region       Hardware Control, Serial/UART, Timers


# Working

[ISA] -> [VM] -> Output


[LANG] -> Compiler -> Assembler -> ISA -> VM -> Output
