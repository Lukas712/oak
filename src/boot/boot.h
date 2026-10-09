/*
 * Camada de boot: a única parte do kernel que conhece a API do
 * bootloader (hoje, o Limine).
 */
#ifndef BOOT_H
#define BOOT_H

#include <stdbool.h>

/*
 * Verifica se o bootloader que carregou o kernel suporta a revisão do
 * protocolo de boot do Limine que pedimos.
 *
 * Retorna true se o boot pode prosseguir normalmente, ou false caso o
 * bootloader seja incompatível.
 */
bool boot_is_supported(void);

#endif /* BOOT_H */
