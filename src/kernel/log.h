/*
 * Log formatado do kernel, enviado à serial.
 *
 * Formatos suportados: %c %s %d %i %u %x %X %p %%, com modificadores
 * de tamanho l, ll e z, e largura com preenchimento de zeros (ex.: %016lx).
 */
#ifndef LOG_H
#define LOG_H

__attribute__((format(printf, 1, 2))) void kprintf(const char *fmt, ...);

#endif /* LOG_H */
