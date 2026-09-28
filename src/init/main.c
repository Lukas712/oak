#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"

// Requisita a versão base v6 do Limine
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

// Marcadores obrigatórios exigidos pelo Limine v6
__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// Trava a CPU em caso de erro fatal (Halt and Catch Fire)
static void hcf(void) {
    for (;;) {
        __asm__("hlt");
    }
}

// Escreve na porta serial COM1 para debug via texto
static inline void outb(uint16_t porta, uint8_t valor) {
    __asm__ volatile ( "outb %0, %1" : : "a"(valor), "Nd"(porta) : "memory");
}

static void imprimir_serial(const char *texto) {
    for (size_t i = 0; texto[i] != '\0'; i++) {
        outb(0x3F8, texto[i]);
    }
}

// Ponto de entrada chamado diretamente pelo Limine
void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    imprimir_serial("\n========================================\n");
    imprimir_serial("[OAK KERNEL] Inicializado com sucesso em C!\n");
    imprimir_serial("========================================\n\n");

    // No TCC, é daqui para frente que você chamará o gerenciador de memória e o escalonador!
    
    hcf();
}