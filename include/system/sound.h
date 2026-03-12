#ifndef _XENO_SOUND_INPUT_H
#define _XENO_SOUND_INPUT_H

#include "psyq/libspu.h"
#include "psyq/libcd.h"

#define NUM_VOICES 24

#define CHANNEL_RIGHT 0
#define CHANNEL_LEFT  1

typedef struct {
    s16 assignedVoice;
    u16 modeFlags;
    s16 priority;
    u16 flags;
    SpuVolume volume;
    u8 unkC[8];
    u16 pitch;
    u8 unk16[6];
    u32 startAddress;
    u32 loopAddress;
    u8 unkAdsr1;
    u8 unkAdsr2;
    u8 unkAdsr3;
    u8 unkAdsr4;
    u8 adsrDR;
    u8 unkAdsr5;
    u8 unkAdsr6;
    u8 adsrSR;
} SoundVoiceData;

#define SOUND_CTL_FLAG_IRQ_HANDLER (1 << 2)

#define SOUND_STATUS_OK 0x0
#define SOUND_ERR_INVALID_SIGNATURE 0x1
#define SOUND_ERR_INVALID_CHECKSUM 0x2 // Maybe
#define SOUND_ERR_UNK_0X4 0x4
#define SOUND_ERR_MANAGER_NOT_IN_LIST 0xF
#define SOUND_ERR_ENTRY_ALREADY_EXISTS 0x15
#define SOUND_ERR_UNEXPECTED_CALLBACK 0x26

#define SOUND_TRANSFER_QUEUE_SIZE 8

#define SOUND_SPU_COMMAND_WRITE 0x1
#define SOUND_SPU_COMMAND_READ 0x2

#define SOUND_WDS_ALLOCATE_AT_ADDRESS 0
#define SOUND_WDS_ALLOCATE_AUTOMATIC -1

#define FILE_SIGNATURE(a, b, c, d) (d<<24)+(c<<16)+(b<<8)+a

typedef void (*SoundCommandCallback_t)(void);

typedef struct {
    /* 0x0 */ u16 unk0; // Flags?
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ void* pPrev;
    /* 0xC */ void* pNext;
} SoundHeapBlockHeader;


#define MAX_SPU_MEMORY_BLOCKS 0xC

#define SPU_MEMORY_FREE 0x0
#define SPU_MEMORY_RESERVED 0x1 // Or last block?
#define SPU_MEMORY_IN_USE 0x80

typedef struct {
    /* 0x0 */ unsigned char flags;
    /* 0x1 */ unsigned char unk1; // Type of memory/data?
    /* 0x2 */ short nextBlockIndex;
    /* 0x4 */ int spuAddress;
    /* 0x8 */ int size;
    /* 0xC */ unsigned int unkC; // Padding?
} SoundSpuMemoryBlock;

// Possible a more general queue to sending commands to the SPU,
// but all supported commands has to do with data transfer.
typedef struct {
    /* 0x0  */ u_short commandType;
    /* 0x2  */ short unk2;
    /* 0x4  */ void* pSpuData;
    /* 0x8  */ u32 pTransferAddress;
    /* 0xC  */ u_long dataSize;
    /* 0x10 */ SoundCommandCallback_t pCallbackFn;
} SoundTransferCommand;

// Possibly a struct which can either be a SMD (Background Music), SED (Sound Effect) or SND entry
struct SoundFile_t {
    /* 0x0 */ unsigned int magic;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u32 unk8; // Size?
    /* 0xC */ u16 unkC; // File format version?
    /* 0xE */ u16 unkE;
    /* 0x10 */ u32 unk10; // smdId?
    /* 0x14 */ unsigned short sedId;
    /* 0x16 */ unsigned short sndId;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ struct SoundFile_t* pNext;
    /* 0x20 */ // starts of 0x2 size offsets to scripts. Pair of scripts for each instrument. 1 script for 1 channel.
};
typedef struct SoundFile_t SoundFile;

struct SoundWDSEntry_t {
    /* 0x0  */ u_char _unk0[0x10];
    /* 0x10 */ unsigned int headerSizeMby;
    /* 0x14 */ unsigned int adpcmDataSize; // Sample size
    /* 0x18 */ unsigned int adpcmDataOffset; // Offset to data to write to SPU
    /* 0x1C */ unsigned short unk1C;
    /* 0x1E */ unsigned short unk1E;
    /* 0x20 */ unsigned short id;
    /* 0x22 */ u_short unk22;
    /* 0x24 */ u_int unk24;
    /* 0x28 */ int spuMemoryAddress; // Optional
    /* 0x2C */ struct SoundWDSEntry_t* pNext;
};
typedef struct SoundWDSEntry_t SoundWDSEntry;

typedef struct {
    s32 currentValue;     // 0x0 - Current interpolated value
    s32 stepIncrement;    // 0x4 - Amount to add each step
    s16 counter;          // 0x8 - Steps remaining
    s16 targetValue;      // 0xA - Final value when counter reaches 0
} AudioInterpolator;

typedef struct {
    u16 active_flag;                // 0x00 - checked for != 0
    u16 status_flags;               // 0x02 - contains bit flags of some sort
    s8 unknown_data[0x23];          // 0x04 - 0x26
    u8 voice_number;                // 0x27 - SPU voice index
    s8 padding[0x8];                // 0x28 - 0x2F
    SoundVoiceData voice_data;      // 0x30 - passed to cancel function
    u8 unk[252];                    // Size of this struct during iteration seems to be 0x158
} AudioElement;

typedef struct AudioManager {
    struct AudioManager* next;                // 0x0
    struct AudioManager* unk_Manager_0x4;     // 0x4
    s32 unk_0x8;                              // 0x8
    s32 unk_0xc;                              // 0xc
    s16 unk_Flags;                            // 0x10
    s8 unk2[2];                               // 0x12
    u8 elementCount;                          // 0x14
    u8 unk_0x15[3];                           // 0x15-0x17
    u8 unk_0x18;                              // 0x18
    u8 unk_0x19;                              // 0x19
    u8 unk_0x1a;                              // 0x1a
    u8 unk_0x1b;                              // 0x1b
    u8 unk_0x1c[4];                           // 0x1c-0x1f
    s32 unk_0x20;                             // 0x20
    s32 unk_0x24;                             // 0x24
    s32 unk_0x28;                             // 0x28
    s32 unk_0x2c;                             // 0x2c
    s16 unk_0x30;                             // 0x30
    s16 unk_0x32;                             // 0x32
    s16 unk_0x34;                             // 0x34
    s16 unk_0x36;                             // 0x36
    s16 unk_0x38;                             // 0x38
    s16 unk_0x3a;                             // 0x3a
    s16 unk_0x3c;                             // 0x3c
    s16 unk_0x3e;                             // 0x3e
    u8 unk_0x40[8];                           // 0x40-0x47
    s32 unk_0x48;                             // 0x48
    u8 unk_0x4c[4];                           // 0x4c-0x4f
    s32 unk_0x50;                             // 0x50
    s32 unk_0x54;                             // 0x54
    s32 unk_0x58;                             // 0x58
    s32 unk_0x5c;                             // 0x5c
    s16 unk_0x60;                             // 0x60
    u8 unk_0x62[2];                           // 0x62-0x63
    AudioInterpolator unk_Interpolator_0x64;  // 0x64
    AudioInterpolator unk_Interpolator_0x70;  // 0x70
    AudioInterpolator unk_Interpolator_0x7c;  // 0x7c
    AudioInterpolator unk_Interpolator_0x88;  // 0x88
    AudioElement elements[24];                // 0x94
} AudioManager;

typedef struct {
    /* 0x00 */ SpuCommonAttr commonAttr;

    // Volume state management
    /* 0x28 */ s16 currentMasterVolume;
    /* 0x2A */ s16 currentCdVolume;
    /* 0x2C */ s16 currentReverbDepth;
    /* 0x2E */ s16 unk_field2;

    /* 0x30 */ AudioInterpolator masterInterpolator;
    /* 0x3C */ AudioInterpolator cdInterpolator;
} SoundVolumeController;

extern SoundVolumeController g_SoundVolumeController;

extern s32 g_ReverbWorkAreaSizes[SPU_REV_MODE_MAX];
extern u8 g_SoundReverbType;
extern u8 g_SoundReverbDelay;
extern u8 g_SoundReverbFeedback;
extern s32 g_SoundReverbMemoryHandle;

extern s32 D_80059404;

// Heap
extern SoundHeapBlockHeader* g_SoundHeapHead;
extern u32 g_SoundHeapEnd;
extern u32 g_SoundHeapSize;

// SPU Transfer Command Queue
extern s16 D_80059548;
extern SoundTransferCommand* g_SoundTransferQueue;
extern u16 g_SoundTransferQueueReadIndex;
extern u16 g_SoundTransferQueueWriteIndex;

// SPU Memory Management
extern SoundSpuMemoryBlock g_SoundSpuMemoryBlocks[MAX_SPU_MEMORY_BLOCKS];

extern s32 g_SoundUploadDestBuffer;
extern s32 g_SoundUploadSourceAddr;
extern s32 g_SoundUploadBytesRemaining;
extern u_long g_unk_SoundEvent; // Event Descriptor

extern SoundFile* g_SoundSedsLinkedList;
extern SoundWDSEntry* g_SoundWdsLinkedList;

extern int g_SoundWdsCurSpuAddress;
extern int g_SoundWdsRemainingBytes;

extern short g_SoundSpuErrorId;
extern long g_unk_VoicesNeedingProcessing;
extern AudioManager* g_SoundAudioManagerListHead;
extern short g_SoundControlFlags;
extern SpuIRQCallbackProc g_SoundSpuIrqCallbackFn;
extern int g_SoundSpuIRQCount;

extern u32 g_SoundKeyOnFlags;
extern u32 g_SoundKeyOffFlags;
extern SoundVoiceData* g_SoundChannels[24];

extern SpuVolume g_SoundReverbDepth;

extern CdlATV g_SoundCdRomAttenuation;

extern s32 SoundCalculateAudioManagerSize(s32 elementCount);
extern void SoundSetVolumeWithPhase(s32, SpuVolume*, s32);

extern void* SoundLoadWdsFile(void*, int);
extern int func_8003BDFC(u32);
extern void func_80039E60(u32);

extern u32 D_80050940[];
extern SoundFile D_80050910;
extern u16 D_80050924[];

#endif