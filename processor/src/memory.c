#include "memory.h"
#include "vm_types.h"
#include "defines.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



bool mem_init(mem_bus* bus){
    bus->ram = (byte_t*)calloc(TOTAL_MEMORY, sizeof(byte_t));
    bus->bus_err = false;

    if(bus->ram == NULL){
        fprintf(stderr, "Fatal Error: Failed to allocate VM memory on heap.\n");
        return false;
    }
    return true;
}

bool mem_free(mem_bus* bus){
    if(bus->ram != NULL){
        free(bus->ram);
        bus->ram = NULL;
        return true;
    }
    fprintf(stderr, "Unable to free VM ram from heap\n");
    return false;
}

void mem_load_binary(mem_bus* bus, const byte_t binary, size_t size, addr_t load_addr){
    if((bus->ram == NULL) || (load_addr + size > TOTAL_MEMORY)){
        bus->bus_err = true;
        return;
    }
   memset(&bus->ram[load_addr], binary, size);
}


byte_t mem_read8(mem_bus* bus, addr_t addr){
    if(addr >= TOTAL_MEMORY){
        bus->bus_err = true;
        return 0;
    }

    return bus->ram[addr];
}
void   mem_write8(mem_bus* bus, addr_t addr, byte_t val){
    if(addr >= TOTAL_MEMORY){
        bus->bus_err = true;
        return;
    }

    if(addr == MMIO_UART_TX){
        putchar((char)val);
        fflush(stdout);
        return;
    }
    bus->ram[addr] = val;
}


word_t mem_read32(mem_bus* bus, addr_t addr){
    if (addr + 3 >= TOTAL_MEMORY || addr % 4 != 0) {
        bus->bus_err = true;
        return 0;
    }

    return ((word_t)bus->ram[addr + 0] << 24) |
    ((word_t)bus->ram[addr + 1] << 16) |
    ((word_t)bus->ram[addr + 2] <<  8) |
    ((word_t)bus->ram[addr + 3] <<  0);

}
void   mem_write32(mem_bus* bus, addr_t addr, word_t val){
    if (addr + 3 >= TOTAL_MEMORY || addr % 4 != 0) {
        bus->bus_err = true;
        return;
    }

    if (addr == MMIO_UART_TX) {
        putchar((char)(val & 0xFF));
        fflush(stdout);
        return;
    }

    bus->ram[addr + 0] = (byte_t)((val >> 24) & 0xFF);
    bus->ram[addr + 1] = (byte_t)((val >> 16) & 0xFF);
    bus->ram[addr + 2] = (byte_t)((val >>  8) & 0xFF);
    bus->ram[addr + 3] = (byte_t)((val >>  0) & 0xFF);
}
