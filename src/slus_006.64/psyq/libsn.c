#include "common.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCopen);

/**
 * @brief Closes a file descriptor opened with PCopen for the SN Systems debugger.
 * 
 * Invokes a hardware exception (break 260) to pause the CPU, handing control 
 * over to the connected SN Systems development hardware/debugger to close the file remotely.
 * 
 * @param fd The file descriptor returned by PCopen.
 */
void PCclose(int fd) {
    __asm__ volatile (
        "addu $a1, $a0, $zero\n\t"
        "break 260\n"
    );
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PClseek);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCcreate);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCinit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCread);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", func_8004C458);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCwrite);
