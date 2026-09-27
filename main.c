#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "defines.h"




int main(void){

     mem_bus* bus = &(mem_bus){.bus_err = false};
    //
    bool init_code = mem_init(bus);
    assert(init_code == 1);
    assert(bus->ram != NULL);
    assert(bus->bus_err == false);
    //
    //
    // mem_write8(bus, RAM_START, 0x41);
    // byte_t val8 = mem_read8(bus, RAM_START);
    //
    // assert(val8 == 0x41);
    // assert(bus->bus_err == false);
    //
    // mem_write8(bus, MMIO_UART_TX, val8);
    //
    //


    const byte_t msg[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0xa};
    size_t len = 14;

    for(int i = 0; i < len; i++){
        mem_write8(bus, RAM_START + i, msg[i]);
    }


    for(int i = 0; i < len; i++){
        byte_t val = mem_read8(bus, RAM_START + i);
        mem_write8(bus, MMIO_UART_TX, val);
    }



    mem_free(bus);





    return 0;
}
