#include "common.h"
#include "field/actor.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_8001FBA4);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67
Co-Authored-By: eagleflo <eagleflo@users.noreply.github.com>

void* func_8001FBA4(SpriteData* pSpriteData, u8* pIndex) {
    u8 index = *pIndex;
    if (!(index & 0x80)) {
        s32 signedIndex = (s8)index;
        return &pSpriteData->stack[pSpriteData->stackIndex + signedIndex];
    }
    return &pSpriteData->field_0x88[index & 0x7F];
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_8001FBE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021AD8);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67

s32 func_80021AD8(s32 color, s32 value) {
    color += value;
    if (color >= 0x100) {
        color = 0xFF;
    } else if (color < 0) {
        color = 0;
    }
    return color;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B14);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B6C);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67
NOTE: Required SpriteData to have own field for prim

void func_80021B6C(SpriteData* pSpriteData) {
    pSpriteData->prim |= 1;
    func_8001F6B0(pSpriteData);
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", SpriteSetColor);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67

void SpriteSetColor(SpriteData *pSpriteData, u8 red, u8 green, u8 blue) {
    pSpriteData->red = red;
    pSpriteData->green = green;
    pSpriteData->blue = blue;
    pSpriteData->prim &= 0xFE;
    func_8001F6B0(pSpriteData);
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021BCC);

void SpriteSetSpecialAnimFile(SpriteData* pSpriteData, void* pAnimFile) {
    pSpriteData->pSpecialAnimFile = pAnimFile;
}

void func_80021BF8(SpriteData* pSpriteData, void* cb) {
    pSpriteData->field_0x68 = cb;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021C00);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU8);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67
Co-Authored-By: Mc-muffin <Mc-muffin@users.noreply.github.com>

u_char AnimScriptStackPopU8(SpriteData* pSpriteData) {
    u_char nStackValue;

    nStackValue = pSpriteData->stack[pSpriteData->stackIndex];
    pSpriteData->stackIndex++;
    return nStackValue;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU16);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU16);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021D3C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021D50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021EBC);

void func_80021FB8(SpriteData* pSpriteData, u8 val) {
    ((u8*)pSpriteData)[0xB0] = val;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FE0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", SpriteSetScale);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67

void SpriteSetScale(SpriteData* pSpriteData, short scale) {
    SpriteDataI1* pBase = pSpriteData->pBase;
    if (pBase) {
        pSpriteData->scale = scale;
        pBase->scale.z = scale;
        pBase->scale.y = scale;
        pBase->scale.x = scale;
        pSpriteData->field_0x3C_6 = 0x1;
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022038);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67

// Recompute transform matrix if flag is set
void func_80022038(SpriteData* pSpriteData) {
    if (pSpriteData->field_0x3C_6 & 0x1) {
        SpriteComputeTransformMatrix(pSpriteData);
        pSpriteData->field_0x3C_6 = 0x0;
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", SpriteComputeTransformMatrix);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67

extern MATRIX D_80018644;

void SpriteComputeTransformMatrix(SpriteData* pSpriteData) {
    VECTOR unused;
    VECTOR scale;
    VECTOR scale2;
    MATRIX rotationMatrix;
    MATRIX transformMatrix;
    
    if (!(pSpriteData->field_0x40 & 1)) {
        scale.vx = pSpriteData->pBase->scale.x;
        scale.vy = pSpriteData->pBase->scale.y;
        scale.vz = pSpriteData->pBase->scale.z;
        RotMatrix(&pSpriteData->pBase->rotation, &pSpriteData->pBase->transformMatrix);
        ScaleMatrixL(&pSpriteData->pBase->transformMatrix, &scale);
    } else {
        transformMatrix = D_80018644;
        scale2.vx = pSpriteData->pBase->scale.x;
        scale2.vy = pSpriteData->pBase->scale.y;
        scale2.vz = pSpriteData->pBase->scale.z;
        ScaleMatrixL(&transformMatrix, &scale2);
        RotMatrix(&pSpriteData->pBase->rotation, &rotationMatrix);
        MulMatrix0(&rotationMatrix, &transformMatrix, &pSpriteData->pBase->transformMatrix);
    }
    if (pSpriteData->field_0x3A) {
        scale.vx = pSpriteData->field_0x3A >> 1;
        scale.vy = pSpriteData->field_0x3A >> 1;
        scale.vz = pSpriteData->field_0x3A >> 1;
        ScaleMatrix(&pSpriteData->pBase->transformMatrix, &scale);
    }
}
*/

// Set animation package
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022224);

extern s32 func_8001EE68(void*);
extern void func_80022224(SpriteAnimPackage*, void*, SVEC2, SVEC2, u32);
extern u8 D_800591AD;

// >= ASPSX 2.56, likely GCC 2.7.2-970404, ASPSX 2.67
void func_800222BC(SpriteData* pSprite, SpriteAnimPackageFileHeader* pAnimPackageFile) {
    SpriteAnimPackage* pAnimPackage;
    SVEC2 unused[2];

    pAnimPackage = pSprite->pVramData;
    if (pAnimPackageFile) {
        if (pAnimPackageFile != pSprite->pCurAnimFile) {
            func_80022224(pAnimPackage, pAnimPackageFile, pAnimPackage->tex, pAnimPackage->clut, pSprite->field_0x3C_1);
            pSprite->pCurAnimFile = pAnimPackageFile;
            pSprite->field_0x3C_3 = 1;
        }

        // Is battle?
        if (D_800591AD) {
            // Is not VRAM Pre-backed?
            if (!func_8001EE68(pAnimPackage->pFrames)) {
                pAnimPackage->tex.y = 0x100;
                pAnimPackage->tex.x = 0x300;
                return;
            }
            pAnimPackage->tex = *(SVEC2*)(pSprite->field_0x7C + 0xE);
        }
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_800223B0);

// Run Sprite Animation VM
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022660);

// Recompute animation speed
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022974);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022A00);
