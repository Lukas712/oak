#include <stdint.h>
#include <stddef.h>
#include "kernel.h"

void hcf(void) {
    for (;;) {
        __asm__("hlt");
    }
}

/* Escreve na porta serial COM1 para debug via texto */
static inline void outb(uint16_t porta, uint8_t valor) {
    __asm__ volatile ( "outb %0, %1" : : "a"(valor), "Nd"(porta) : "memory");
}

static void imprimir_serial(const char *texto) {
    for (size_t i = 0; texto[i] != '\0'; i++) {
        outb(0x3F8, texto[i]);
    }
}

void kernel_main(void) {
    imprimir_serial("\n========================================\n");
    imprimir_serial("[OAK KERNEL] Inicializado com sucesso em C!\n");
    imprimir_serial("========================================\n\n");

    hcf();
}
