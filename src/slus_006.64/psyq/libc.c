#include "common.h"

extern u_long g_RandomSeed;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", bzero);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libc", memchr);
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
