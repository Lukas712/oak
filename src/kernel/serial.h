/*
 * Driver da porta serial COM1 (UART 16550), usada para depuração.
 */
#ifndef SERIAL_H
#define SERIAL_H

/* Configura a COM1: 115200 baud, 8 bits, sem paridade, 1 stop bit, FIFO ligado. */
void serial_init(void);

/* Envia um byte, aguardando o buffer de transmissão ficar livre. */
void serial_putc(char c);

/* Envia uma string terminada em '\0'. */
void serial_puts(const char *s);

#endif /* SERIAL_H */
