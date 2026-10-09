#include <stdint.h>
#include "boot.h"
#include "limine.h"

/* Requisita a versão base v6 do Limine */
__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

/* Marcadores obrigatórios exigidos pelo Limine v6 */
__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

bool boot_is_supported(void) {
    return LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision);
}
