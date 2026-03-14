#include "common.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"

extern void memcpy(u8*, u8*, int);

extern char g_GraphDebugLevel;
extern void (*g_DrawSyncCallbackFn)();
extern int (*g_GpuPrintf)(char*, ...);

extern DRAWENV g_GpuDrawEnv;
extern DISPENV g_GpuDispEnv;

extern GpuPackage *g_GpuPkg;

DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h) {
    int nVideoMode;

    nVideoMode = GetVideoMode();
    env->clip.x = x;
    env->clip.y = y;
    env->clip.w = w;
    env->clip.h = h;
    env->tw.x = 0;
    env->tw.y = 0;
    env->tw.w = 0;
    env->tw.h = 0;
    env->r0 = 0;
    env->g0 = 0;
    env->b0 = 0;
    env->dtd = 1;
    
    if (nVideoMode != MODE_NTSC)
        env->dfe  = h < 0x121;
    else
        env->dfe  = h < 0x101;

    env->ofs[0] = x;
    env->ofs[1] = y;
    env->tpage = 0xA;
    env->isbg = 0;
    return env;
}

DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h) {
    env->disp.x = x;
    env->disp.y = y;
    env->disp.w = w;
    env->disp.h = h;
    env->screen.x = 0;
    env->screen.y = 0;
    env->screen.w = 0;
    env->screen.h = 0;
    env->isrgb24 = 0;
    env->isinter = 0;
    env->pad1 = 0;
    env->pad0 = 0;
    return env;
}

u_short GetTPage(int tp, int abr, int x, int y) {
    return getTPage(tp, abr, x, y);
}

u_short GetClut(int x, int y) {
    return getClut(x, y);
}

void DumpTPage(u_short tpage) {
    dumpTPage(tpage);
}

void DumpClut(u_short clut) {
    dumpClut(clut);
}

void* NextPrim(void *p) {
    return nextPrim(p);
}

int IsEndPrim(void *p) {
    return isendprim(p);
}

void AddPrim(void *ot, void *p) {
    addPrim(ot, p);
}

void AddPrims(void *ot, void *p0, void *p1) {
    addPrims(ot, p0, p1);
}

void CatPrim(void *p0, void *p1) {
    setaddr(p0, p1);
}

void TermPrim(void *p) {
    termPrim(p);
}

void SetSemiTrans(void *p, int abe) {
    setSemiTrans(p, abe);
}

void SetShadeTex(void *p, int tge) {
    setShadeTex(p, tge);
}

void SetPolyF3(POLY_F3 *p) {
    setlen(p, 4);
    setcode(p, 0x20);
}

void SetPolyFT3(POLY_FT3 *p) {
    setlen(p, 7);
    setcode(p, 0x24);
}

void SetPolyG3(POLY_G3 *p) {
    setlen(p, 6);
    setcode(p, 0x30);
}

void SetPolyGT3(POLY_GT3 *p) {
    setlen(p, 9);
    setcode(p, 0x34);
}

void SetPolyF4(POLY_F4 *p) {
    setlen(p, 5);
    setcode(p, 0x28);
}

void SetPolyFT4(POLY_FT4 *p) {
    setlen(p, 9);
    setcode(p, 0x2C);
}

void SetPolyG4(POLY_G4 *p) {
    setlen(p, 8);
    setcode(p, 0x38);
}

void SetPolyGT4(POLY_GT4 *p) {
    setlen(p, 12);
    setcode(p, 0x3C);
}

void SetSprt8(SPRT_8 *p) {
    setlen(p, 3);
    setcode(p, 0x74);
}

void SetSprt16(SPRT_16 *p) {
    setlen(p, 3);
    setcode(p, 0x7C);
}

void SetSprt(SPRT *p) {
    setlen(p, 4);
    setcode(p, 0x64);
}

void SetTile1(TILE_1 *p) {
    setlen(p, 2);
    setcode(p, 0x68);
}

void SetTile8(TILE_8 *p) {
    setlen(p, 2);
    setcode(p, 0x70);
}

void SetTile16(TILE_16 *p) {
    setlen(p, 2);
    setcode(p, 0x78);
}

void SetTile(TILE *p) {
    setlen(p, 3);
    setcode(p, 0x60);
}

void SetLineF2(LINE_F2 *p) {
    setlen(p, 3);
    setcode(p, 0x40);
}

void SetLineG2(LINE_G2 *p) {
    setlen(p, 4);
    setcode(p, 0x50);
}

void SetLineF3(LINE_F3 *p) {
    setlen(p, 5);
    setcode(p, 0x48);
    p->pad = 0x55555555;
}

void SetLineG3(LINE_G3 *p) {
    setlen(p, 7);
    setcode(p, 0x58);
    p->pad = 0x55555555;
}

void SetLineF4(LINE_F4 *p) {
    setlen(p, 6);
    setcode(p, 0x4c);
    p->pad = 0x55555555;
}

void SetLineG4(LINE_G4 *p) {
    setlen(p, 9);
    setcode(p, 0x5c);
    p->pad = 0x55555555;
}

void SetDrawTPage(DR_TPAGE *p, int dfe, int dtd, int tpage) {
    setlen(p, 1);
    ((u_long *)(p))[1] = _get_mode(dfe, dtd, tpage);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetDrawMove);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043EAC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043F18);

void DumpDrawEnv(DRAWENV *env) {
    g_GpuPrintf("clip (%3d,%3d)-(%d,%d)\n", env->clip.x, env->clip.y, env->clip.w, env->clip.h);
    g_GpuPrintf("ofs  (%3d,%3d)\n", env->ofs[0], env->ofs[1]);
    g_GpuPrintf("tw   (%d,%d)-(%d,%d)\n", env->tw.x, env->tw.y, env->tw.w, env->tw.h);
    g_GpuPrintf("dtd   %d\n", env->dtd);
    g_GpuPrintf("dfe   %d\n", env->dfe);
    dumpTPage(env->tpage);
}

void DumpDispEnv(DISPENV *env) {
    g_GpuPrintf("disp   (%3d,%3d)-(%d,%d)\n", env->disp.x, env->disp.y, env->disp.w, env->disp.h);
    g_GpuPrintf("screen (%3d,%3d)-(%d,%d)\n", env->screen.x, env->screen.y, env->screen.w, env->screen.h);
    g_GpuPrintf("isinter %d\n", env->isinter);
    g_GpuPrintf("isrgb24 %d\n", env->isrgb24);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ResetGraph);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetGraphReverse);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetGraphDebug);
/*
int SetGraphDebug(int level) {
    int nPrev = g_GraphDebugLevel;

    g_GraphDebugLevel = level;
    if (level) {
        g_GpuPrintf("SetGraphDebug:level:%d,type:%d reverse:%d\n", level, DAT_800568d0, DAT_800568d3);
    }

    return nPrev;
}
*/

// SetGrapQue ?
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_8004440C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_800444B8);

int GetGraphDebug(void) {
    return g_GraphDebugLevel;
}

extern char D_80019118; // "DrawSyncCallback(%08x)...\n"
u_long DrawSyncCallback(void (*pCallbackFn)()) {
    void (*pPrevCallbackFn)();

    if (g_GraphDebugLevel >= 2)
        g_GpuPrintf(&D_80019118, pCallbackFn);

    pPrevCallbackFn = g_DrawSyncCallbackFn;
    g_DrawSyncCallbackFn = pCallbackFn;
    return (u_long) pPrevCallbackFn;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetDispMask);

extern char D_80019148; // "DrawSync(%d)...\n"
/**
 * @brief Wait for all drawing to finish, or check drawing status.
 * @param mode 0 = block until complete, non-zero = return remaining count
 * @return Number of positions remaining in the command buffer
 */
int DrawSync(int mode) {
    if (g_GraphDebugLevel >= 2)
        g_GpuPrintf(&D_80019148, mode);

    return g_GpuPkg->drawSync(mode);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_8004463C);

extern char D_80019180; // "ClearImage"
/**
 * @brief Fill a VRAM rectangle with a solid color.
 * @param rect Target rectangle on VRAM
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return Transfer status
 */
int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {
    func_8004463C(&D_80019180, rect);
    return g_GpuPkg->dmaTransfer(g_GpuPkg->dmaClearCfg, rect, 8, (b << 16) | (g << 8) | r);
}

/**
 * @brief Fill a VRAM rectangle with a solid color, setting the STP bit.
 *
 * Identical to ClearImage but sets bit 31 (semi-transparency processing)
 * on every written pixel.
 *
 * @param rect Target rectangle on VRAM
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return Transfer status
 */
int ClearImage2(RECT *rect, u_char r, u_char g, u_char b) {
    int bv, color;
    func_8004463C(&D_80019180, rect);
    bv = b << 16;
    color = (g << 8) | 0x80000000;
    return g_GpuPkg->dmaTransfer(g_GpuPkg->dmaClearCfg, rect, 8, bv | color | r);
}

extern char D_8001918C; // "LoadImage"
/**
 * @brief Transfer a rectangular image area from main memory to VRAM.
 * @param rect Destination rectangle on VRAM
 * @param p Source pixel data in main memory
 * @return Transfer status
 */
int LoadImage(RECT *rect, u_long *p) {
    func_8004463C(&D_8001918C, rect);
    return g_GpuPkg->dmaTransfer(g_GpuPkg->dmaLoadCfg, rect, 8, p);
}

extern char D_80019198; // "StoreImage"
/**
 * @brief Transfer a rectangular image area from VRAM to main memory.
 * @param rect Source rectangle on VRAM
 * @param p Destination buffer in main memory
 * @return Transfer status
 */
int StoreImage(RECT *rect, u_long *p) {
    func_8004463C(&D_80019198, rect);
    return g_GpuPkg->dmaTransfer(g_GpuPkg->dmaStoreCfg, rect, 8, p);
}

extern char D_800191A4; // "MoveImage"
extern u_long D_80056980;
extern u_long D_80056984;
extern u_long D_80056988;
/**
 * @brief Transfer a VRAM rectangle to a new position via GPU DMA.
 * @param rect Source rectangle in VRAM
 * @param x Destination X coordinate
 * @param y Destination Y coordinate
 * @return Transfer status, or -1 if rect has zero width or height
 */
int MoveImage(RECT *rect, int x, int y) {
    register int dest asm("$2");
    u_long *buf;

    func_8004463C(&D_800191A4, rect);

    dest = rect->w;
    if (dest == 0) return -1;

    dest = rect->h;
    if (dest) {
        dest = y << 16;
        dest |= (x & 0xFFFF);
        buf = &D_80056980;
        /*
         * Equivalent C (without scheduling workarounds):
         *
         *   D_80056984 = dest;
         *   *buf = *(u_long *)rect;
         *   dest = *((u_long *)rect + 1);
         *   D_80056988 = dest;
         *   dest = g_GpuPkg->dmaTransfer(g_GpuPkg->dmaOTagCfg, buf - 2, 0x14, 0);
         *
         * Inline asm is used to pin the 3rd/4th args and control instruction
         * scheduling so GCC fills load delay slots the same way as the original
         * compiler. Without this, GCC hoists the dmaOTagCfg load into a delay
         * slot where the original placed `move a3, zero`.
         */
        {
            u_long rectData = *(u_long *)rect;
            register GpuPackage *pkg asm("$3") = g_GpuPkg;
            register int a2 asm("$6");
            register int a3 asm("$7");
            __asm__("addiu $6, $0, 0x14");
            D_80056984 = dest;
            *buf = rectData;
            dest = *((u_long *)rect + 1);
            __asm__("move $7, $0");
            D_80056988 = dest;
            __asm__("");
            dest = pkg->dmaTransfer(pkg->dmaOTagCfg, buf - 2, a2, a3);
        }
        return dest;
    }
    return -1;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ClearOTag);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ClearOTagR);

/**
 * @brief Send a single GPU primitive directly, bypassing the ordering table.
 * @param p Pointer to the primitive to draw
 */
void DrawPrim(void *p) {
    int len;

    len = ((u_char *)p)[3];
    g_GpuPkg->drawSync(0);
    g_GpuPkg->sendGP0((u_char *)p + 4, len);
}

extern char D_800191E0; // "DrawOTag(%08x)...\n"
/**
 * @brief Send an ordering table to the GPU for rendering.
 * @param p Pointer to the ordering table to draw
 */
void DrawOTag(u_long *p) {
    if (g_GraphDebugLevel >= 2)
        g_GpuPrintf(&D_800191E0, p);

    g_GpuPkg->dmaTransfer(g_GpuPkg->dmaOTagCfg, p, 0, 0);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", PutDrawEnv);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", DrawOTagEnv);

DRAWENV* GetDrawEnv(DRAWENV* env) {
    memcpy(env, &g_GpuDrawEnv, sizeof(DRAWENV));
    return env;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", PutDispEnv);

DISPENV* GetDispEnv(DISPENV* env) {
    memcpy(env, &g_GpuDispEnv, sizeof(DISPENV));
    return env;
}
