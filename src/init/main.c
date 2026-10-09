#include "../boot/boot.h"
#include "../kernel/kernel.h"

/*
 * Ponto de entrada chamado diretamente pelo Limine.
 *
 */
void kmain(void) {
    if (!boot_is_supported()) {
        hcf();
    }

    kernel_main();
}
