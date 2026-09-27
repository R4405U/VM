#include "vm_types.h"
#include <stdbool.h>
#include <stddef.h>

#ifndef MEMORY_H
#define MEMORY_H


typedef struct {
    byte_t* ram;
    bool bus_err;
} mem_bus;


bool mem_init(mem_bus* bus);
bool mem_free(mem_bus* bus);
void mem_load_binary(mem_bus* bus, const byte_t binary, size_t size, addr_t addr);

byte_t mem_read8(mem_bus* bus, addr_t addr);
void   mem_write8(mem_bus* bus, addr_t addr, byte_t val);


word_t mem_read32(mem_bus* bus, addr_t addr);
void  mem_write32(mem_bus* bus, addr_t addr, word_t val);



#endif // MEMORY_H

