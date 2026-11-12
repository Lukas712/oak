#include <stdint.h>
#include "serial.h"

#define COM1 0x3F8

/* Offsets dos registradores em relação à porta base. */
#define REG_DATA 0 /* dados (DLAB=0) / divisor baixo (DLAB=1) */
#define REG_IER  1 /* habilitação de interrupções / divisor alto (DLAB=1) */
#define REG_FCR  2 /* controle de FIFO */
#define REG_LCR  3 /* controle de linha */
#define REG_MCR  4 /* controle de modem */
#define REG_LSR  5 /* status de linha */

#define LSR_THR_EMPTY 0x20

static inline void outb(uint16_t porta, uint8_t valor) {
    __asm__ volatile ("outb %0, %1" : : "a"(valor), "Nd"(porta) : "memory");
}

static inline uint8_t inb(uint16_t porta) {
    uint8_t valor;
    __asm__ volatile ("inb %1, %0" : "=a"(valor) : "Nd"(porta) : "memory");
    return valor;
}

void serial_init(void) {
    outb(COM1 + REG_IER, 0x00); /* sem interrupções: usamos polling */
    outb(COM1 + REG_LCR, 0x80); /* DLAB=1 para programar o divisor */
    outb(COM1 + REG_DATA, 0x01); /* divisor = 1 -> 115200 baud */
    outb(COM1 + REG_IER, 0x00);
    outb(COM1 + REG_LCR, 0x03); /* 8N1, DLAB=0 */
    outb(COM1 + REG_FCR, 0xC7); /* FIFO ligado e limpo, gatilho de 14 bytes */
    outb(COM1 + REG_MCR, 0x03); /* DTR + RTS */
}

void serial_putc(char c) {
    while ((inb(COM1 + REG_LSR) & LSR_THR_EMPTY) == 0) {
    }
    outb(COM1 + REG_DATA, (uint8_t)c);
}

void serial_puts(const char *s) {
    for (; *s != '\0'; s++) {
        serial_putc(*s);
    }
}
