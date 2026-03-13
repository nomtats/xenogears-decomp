#include "common.h"
#include <psyq/libgte.h>


INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", InitGeom);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048C4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048CDC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048D68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048DA8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048DD8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048E94);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", LoadAverage12);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80048FCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004901C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_800490A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", LoadAverageByte);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", LoadAverageCol);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", MulMatrix0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", CompMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004947C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_800495DC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", PushMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", PopMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ScaleMatrixL);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetMulMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004998C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", MulMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", MulMatrix2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ApplyMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ApplyMatrixSV);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", TransMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ScaleMatrix);

/**
 * @brief Sets the 3x3 rotation matrix in the GTE (Geometry Transformation Engine).
 * 
 * Transfers the 3x3 rotation matrix (m[3][3] -> 5 halfwords, padded to 5 words)
 * from the MATRIX structure in main memory into Coprocessor 2 (GTE) control registers.
 * 
 * @param m Pointer to the MATRIX structure containing rotation data.
 */
void SetRotMatrix(MATRIX *m) {
    __asm__ volatile (
        "lw $t0, 0(%0)\n\t"
        "lw $t1, 4(%0)\n\t"
        "lw $t2, 8(%0)\n\t"
        "lw $t3, 12(%0)\n\t"
        "lw $t4, 16(%0)\n\t"
        "ctc2 $t0, $0\n\t"
        "ctc2 $t1, $1\n\t"
        "ctc2 $t2, $2\n\t"
        "ctc2 $t3, $3\n\t"
        "ctc2 $t4, $4"
        :
        : "r"(m)
        : "t0", "t1", "t2", "t3", "t4"
    );
}



INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetLightMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetColorMatrix);

/**
 * @brief Sets the 3D translation vector in the GTE (Geometry Transformation Engine).
 * 
 * Transfers the translation vector (t[3] -> 3 words) from the MATRIX structure
 * in main memory into Coprocessor 2 (GTE) control registers $5, $6, and $7
 * (TRX, TRY, TRZ).
 * 
 * @param m Pointer to the MATRIX structure containing translation data.
 */
void SetTransMatrix(MATRIX *m) {
    __asm__ volatile (
        "lw $t0, 20(%0)\n\t"
        "lw $t1, 24(%0)\n\t"
        "lw $t2, 28(%0)\n\t"
        "ctc2 $t0, $5\n\t"
        "ctc2 $t1, $6\n\t"
        "ctc2 $t2, $7"
        :
        : "r"(m)
        : "t0", "t1", "t2"
    );
}


INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetVertex0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetVertex1);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetVertex2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetVertexTri);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_80049FFC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A010);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A024);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A030);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A044);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A05C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A070);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A084);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A098);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A0A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A0B0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ReadGeomOffset);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", ReadGeomScreen);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetBackColor);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetFarColor);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetGeomOffset);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SetGeomScreen);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A15C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A180);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A19C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A1B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A1F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A218);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A260);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A280);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A2C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A2EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A310);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A320);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A33C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A364);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A38C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A3C8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A3EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A414);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A43C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A45C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A480);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A4D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A54C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A57C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", func_8004A5B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SquareSL12);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", SquareSL0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotTransPers);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotTransPers3);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotTrans);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", NormalClip);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotTransPers4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotAverage4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotAverageNclip4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", TransposeMatrix);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotMatrixYXZ);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotMatrixZYX);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotMatrixX);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotMatrixY);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgte", RotMatrixZ);
