#ifndef PROCESSOR_DEFINES_H
#define PROCESSOR_DEFINES_H


#define TOTAL_MEMORY (1024*1024) // in bytes


#define BOOTLOADER_START    0x00000000
#define RAM_START           0x00000200
#define STACK_REGION_BOTTOM 0x000D0000
#define STACK_INIT_TOP      0x000E0000
#define VRAM_START          0x000E0000
#define VRAM_END            0x000EFF1F
#define MMIO_START          0x000F0000
#define MMIO_UART_TX        0x000F0000
#define MMIO_END            0x000FFFFF


#define TOTAL_REGS 23



#endif // PROCESSOR_DEFINES_H
