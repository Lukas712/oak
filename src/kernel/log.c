#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "log.h"
#include "serial.h"

/* Escreve `valor` na base indicada, com preenchimento até `largura`. */
static void print_unsigned(uint64_t valor, unsigned base, bool maiusculas,
                           int largura, bool zeros, bool negativo) {
    const char *digitos = maiusculas ? "0123456789ABCDEF" : "0123456789abcdef";
    char buf[64];
    int n = 0;

    do {
        buf[n++] = digitos[valor % base];
        valor /= base;
    } while (valor != 0);

    int total = n + (negativo ? 1 : 0);

    if (negativo && zeros) {
        serial_putc('-');
    }
    for (; total < largura; total++) {
        serial_putc(zeros ? '0' : ' ');
    }
    if (negativo && !zeros) {
        serial_putc('-');
    }
    while (n > 0) {
        serial_putc(buf[--n]);
    }
}

static void vkprintf(const char *fmt, va_list args) {
    for (; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            serial_putc(*fmt);
            continue;
        }
        fmt++;

        bool zeros = false;
        int largura = 0;
        int tamanho = 0; /* 0 = int, 1 = long, 2 = long long, 3 = size_t */

        if (*fmt == '0') {
            zeros = true;
            fmt++;
        }
        while (*fmt >= '0' && *fmt <= '9') {
            largura = largura * 10 + (*fmt - '0');
            fmt++;
        }
        if (*fmt == 'l') {
            tamanho = 1;
            fmt++;
            if (*fmt == 'l') {
                tamanho = 2;
                fmt++;
            }
        } else if (*fmt == 'z') {
            tamanho = 3;
            fmt++;
        }

        switch (*fmt) {
        case 'c':
            serial_putc((char)va_arg(args, int));
            break;
        case 's': {
            const char *s = va_arg(args, const char *);
            serial_puts(s != NULL ? s : "(null)");
            break;
        }
        case 'd':
        case 'i': {
            int64_t v;
            if (tamanho == 0) {
                v = va_arg(args, int);
            } else if (tamanho == 3) {
                v = (int64_t)va_arg(args, ptrdiff_t);
            } else {
                v = va_arg(args, long long);
            }
            bool neg = v < 0;
            uint64_t abs = neg ? (uint64_t)0 - (uint64_t)v : (uint64_t)v;
            print_unsigned(abs, 10, false, largura, zeros, neg);
            break;
        }
        case 'u':
        case 'x':
        case 'X': {
            uint64_t v;
            if (tamanho == 0) {
                v = va_arg(args, unsigned int);
            } else if (tamanho == 3) {
                v = va_arg(args, size_t);
            } else {
                v = va_arg(args, unsigned long long);
            }
            unsigned base = (*fmt == 'u') ? 10 : 16;
            print_unsigned(v, base, *fmt == 'X', largura, zeros, false);
            break;
        }
        case 'p':
            serial_puts("0x");
            print_unsigned((uint64_t)(uintptr_t)va_arg(args, void *), 16, false,
                           16, true, false);
            break;
        case '%':
            serial_putc('%');
            break;
        default:
            serial_putc('%');
            if (*fmt != '\0') {
                serial_putc(*fmt);
            } else {
                return;
            }
            break;
        }
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vkprintf(fmt, args);
    va_end(args);
}
