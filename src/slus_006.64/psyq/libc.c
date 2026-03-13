#include "common.h"

extern u_long g_RandomSeed;

/**
 * @brief Zeroes out a block of memory.
 * 
 * @param pDst Destination buffer.
 * @param size Number of bytes to zero out.
 * @return void* Pointer to the destination buffer, or NULL if it was null.
 */
void* bzero(u_char* pDst, int size) {
    u_char* start;

    /* To match PsyQ GCC's exact branch delay slot utilization where
       the original jump locations place specific return variables in
       delay slots without crossjumping (merging), flat goto statements 
       are required here rather than standard structured C loops. */
    if (pDst == NULL) {
        return NULL;
    }

    if (size > 0) {
        start = pDst;
        goto loop;
    }
    
    start = NULL;
    goto end;

loop:
    do {
        *pDst = 0;
        size--;
        pDst++;
    } while (size > 0);

end:
    return start;
}
/**
 * @brief Locates the first occurrence of a character in a block of memory.
 * 
 * @param pDst Destination buffer.
 * @param value Character to locate.
 * @param size Number of bytes to check.
 * @return void* Pointer to the matching byte, or NULL if it was null or not found.
 */
void* memchr(u_char* pDst, int value, int size) {
    if (pDst == NULL) {
        return NULL;
    }

    if (size <= 0) {
        return NULL;
    }
    
    /* To match PsyQ GCC's exact branch delay slot utilization, flat goto
       statements are required here. A standard `while (size-- > 0)` loop
       fails to generate the explicit initial jump to the loop condition 
       with the `size--` decrement firmly seated in its delay slot. */
    goto loop_check;

match:
    return pDst - 1;

loop_check:
    size--;
    if (size < 0) {
        return NULL;
    }
    value &= 0xFF;

loop:
    if (*pDst++ == value) {
        goto match;
    }
    size--;
    if (size >= 0) {
        goto loop;
    }

    return NULL;
}
/**
 * @brief Copies size bytes from pSrc to pDst.
 * 
 * @param pDst Destination buffer.
 * @param pSrc Source buffer.
 * @param size Number of bytes to copy.
 * @return void* Pointer to the destination buffer (pDst), or NULL if it was null.
 */
void* memcpy(u_char* pDst, u_char* pSrc, int size) {
    u_char* start;

    if (pDst == NULL) {
        return NULL;
    }

    /* Assigning `start` after the first branch allows the compiler to push 
       `start = pDst` into the branch delay slot of `blez $a2` (the size check)
       instead of at the very beginning of the function. */
    start = pDst;
    if (size > 0) {
        do {
            *pDst = *pSrc;
            pSrc++;
            size--;
            pDst++;
        } while (size > 0);
    }

    return start;
}

void* memmove(u_char* pDst, u_char* pSrc, int size) {
    if (pDst >= pSrc) {
        while (size-- > 0) {
            pDst[size] = pSrc[size];
        }
    } else {
        while (size-- > 0) {
            *pDst++ = *pSrc++;
        }
    }

    return pDst;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", memset);

int rand(void) {
    u_long nNext;

    nNext = (g_RandomSeed * 0x41C64E6D) + 0x3039;
    g_RandomSeed = nNext;
    return (nNext >> 0x10) & 0x7FFF;
}

void srand(u_long seed) {
    g_RandomSeed = seed;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FA78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FB20);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", func_8003FB84);

int strlen(char* pString) {
    int nLen;
    char chCur;

    nLen = 0;
    if (pString == NULL)
        return 0;

    while (chCur = *pString, pString++, chCur != NULL)
        nLen++;
    return nLen;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", Sprintf);
