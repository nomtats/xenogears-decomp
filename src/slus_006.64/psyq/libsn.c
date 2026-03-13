#include "common.h"

/**
 * @brief Opens a file using the SN Systems development environment.
 * 
 * Invokes a hardware exception (break 259) to pause the CPU, allowing the 
 * connected SN Systems debugger to remotely open the specified file.
 * 
 * @param name The path/name of the file to open.
 * @param flags Open mode flags (e.g., read, write, append).
 * @param perms File permissions (often ignored or default on development hardware).
 * @return int A file descriptor on success, or -1 on failure.
 */
int PCopen(int name, int flags, int perms) {
    register int result __asm__("v0");
    __asm__ volatile (
        "addu       $a2,$a1,$zero\n\t"
        "addu       $a1,$a0,$zero\n\t"
        "break      259\n\t"
        "beqz       $v0,1f\n\t"
        " addu      $v0,$v1,$zero\n\t"
        "addiu      $v0,$zero,-1\n"
        "1:\n"
    );
    return result;
}

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

/**
 * @brief Sets the file position indicator for the SN Systems debugger connection.
 * 
 * Invokes a hardware exception (break 263) to command the debugger to alter the
 * seek position for the given file descriptor.
 * 
 * @param fd The file descriptor.
 * @param offset The number of bytes to offset.
 * @param mode The whence/mode for sweeping (SEEK_SET, SEEK_CUR, SEEK_END).
 * @return int The new file offset on success, or -1 on failure.
 */
int PClseek(int fd, int offset, int mode) {
    register int result __asm__("v0");
    __asm__ volatile (
        "addu       $a3,$a2,$zero\n\t"
        "addu       $a2,$a1,$zero\n\t"
        "addu       $a1,$a0,$zero\n\t"
        "break      263\n\t"
        "beqz       $v0,1f\n\t"
        " addu      $v0,$v1,$zero\n\t"
        "addiu      $v0,$zero,-1\n"
        "1:\n"
    );
    return result;
}

/**
 * @brief Creates a new file over the SN Systems debugger connection.
 * 
 * Invokes a hardware exception (break 258) to command the debugger to create
 * a new file on the host PC. 
 * 
 * @param name The path/name of the file to create.
 * @param flags Usually ignored or unused compared to PCopen.
 * @param perms File permissions.
 * @return int A file descriptor on success, or -1 on failure.
 */
int PCcreate(int name, int flags, int perms) {
    register int result __asm__("v0");
    __asm__ volatile (
        "addu       $a1,$a0,$zero\n\t"
        "addu       $a2,$zero,$zero\n\t"
        "break      258\n\t"
        "beqz       $v0,1f\n\t"
        " addu      $v0,$v1,$zero\n\t"
        "addiu      $v0,$zero,-1\n"
        "1:\n"
    );
    return result;
}

/**
 * @brief Initializes the SN Systems communication system.
 * 
 * Prepares the development hardware context for PC I/O by executing break 257.
 */
void PCinit(void) {
    __asm__ volatile (
        "break      257\n"
    );
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCread);

/**
 * @brief Primitive internal read step for PCread.
 * 
 * Triggers hardware breakpoint 261 to command the SN Systems host to read
 * a chunk of data. This wrapper handles the raw communication register checks.
 * 
 * @param unk Always 0. Unused/reserved flag.
 * @param fd The file descriptor to read from.
 * @param len The maximum number of bytes to read in this chunk (max 0x8000).
 * @param buff The buffer to read the data into.
 * @return int The amount of newly read bytes, or -1 on error.
 */
int func_8004C458(int unk, int fd, int len, char *buff) {
    register int result __asm__("v0");
    __asm__ volatile (
        "break      261\n\t"
        "beqz       $v0,1f\n\t"
        " addu      $v0,$v1,$zero\n\t"
        "addiu      $v0,$zero,-1\n"
        "1:\n"
    );
    return result;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libsn", PCwrite);
