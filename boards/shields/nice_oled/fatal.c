#include <zephyr/fatal.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>

// Zephyr's default handler halts the whole system with interrupts locked, which
// leaves the keyboard frozen until power cycle. Reboot instead so it recovers.
void k_sys_fatal_error_handler(unsigned int reason, const struct arch_esf *esf)
{
    ARG_UNUSED(reason);
    ARG_UNUSED(esf);
    sys_reboot(SYS_REBOOT_COLD);
    CODE_UNREACHABLE;
}
