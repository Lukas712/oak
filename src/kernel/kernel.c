#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "log.h"
#include "serial.h"

void hcf(void) {
    for (;;) {
        __asm__("hlt");
    }
}

void kernel_main(void) {
    serial_init();

    kprintf("\n========================================\n");
    kprintf("[OAK KERNEL] Inicializado com sucesso em C!\n");
    kprintf("========================================\n\n");

    hcf();
}
