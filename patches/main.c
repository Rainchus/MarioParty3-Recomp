#include "patches.h"
#ifndef RECOMP
#define RECOMP
#endif
#include "common.h"

typedef struct unkProcessStruct {
    s16 stat;
    s16 unk2;
    Process* processInstance;
    void (*destructor)();
} unkProcessStruct;

extern s16 D_800A1774_A2374;
extern s16 D_800A1776_A2376;
extern s16 D_800A1778_A2378;
extern unkProcessStruct* D_800A177C_A237C;
extern s16 D_800A1780_A2380;
extern s8 D_800D1710_D2310;

void omDestroyPrcObj(void);
void func_8000BA00_C600(void);
void func_8000BBFC_C7FC(s32, s16);
void func_800166D0_172D0(void);
void func_8001AFE4_1BBE4(void);
void func_80021AF4_226F4(void);
void omInsertObj(omObjData * obj);
void omAddMember(u16 group, omObjData * obj);
void func_8004A354_4AF54(omObjData*);
void func_8004A444_4B044(s32);
void func_8004A468_4B068(u16, u16, u16);
void func_8004CEA4_4DAA4();
extern u8 D_800A16B0_A22B0[][2];
extern u8 D_800A1740_A2340[][2];
extern u16 D_800A1756_A2356;
extern s32 omprevovl;
extern u16 omovlstat;
extern s8 D_800A176C_A236C;
extern omObjData* omobjall;
extern u8 D_800A1784_A2384;
extern s16 D_800A18A8_A24A8[][2];
extern u8 D_800A18B7_A24B7[];
extern s16 D_800A1908_A2508;
extern s16 D_800A190C_A250C;
extern u16 D_800B23C2_B2FC2;
extern u16 D_800B23C4_B2FC4;
extern s16 D_800B23C6_B2FC6;
extern s16 D_800B23C8_B2FC8;
extern u16 D_800D1100_D1D00;

typedef struct UnkomOvl {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    char unk_06[2]; //unused pad?
} UnkomOvl;

typedef struct {
    u16 next_idx;
    u16 max_objs;
    u16 num_objs;
    u16* next;
    omObjData** objs;
} omObjGroup;

extern UnkomOvl D_800A1920_A2520[];

extern f32 D_800CB890_CC490;
extern s16 omobjlast;
extern s16 ommaxobjs;
extern u8 D_800CCF52_CDB52;
extern s16 omnumobjs;
extern f32 D_800CCF98_CDB98[];
extern omObjGroup omgroup[];
extern u16 D_800CD2F4_CDEF4;
extern s32 omovl;
extern u16 D_800CE202_CEE02;
extern s16 D_800D0A3A_D163A;
extern s32 D_800D2118_D2D18;
extern u16 D_800D4082_D4C82;
extern f32 D_800D4198_D4D98[];
extern f32 D_800D5210_D5E10;
extern u16 D_800D530C_D5F0C;
extern s16 omobjfirst;
extern s32 D_800D6A40_D7640;
extern u16 D_800D6A56_D7656;
extern s8 D_800D6A90_D7690;
extern s16 omnextidx;
extern s8 D_800A1762_A2362;
extern u16 D_800B23C0_B2FC0;
extern u16 D_800CD050_CDC50;
extern s16 omovlhisidx;
extern omOvlHisData omovlhis[12];
extern omOvlHisData D_800D20F0_D2CF0[5];

RECOMP_PATCH s32 omOvlCallEx(s32 ovlID, s16 event, u16 stat) {
    recomp_printf("omOvlCallEx called\n");
    omOvlHisData* history;

    // history is full, cannot call overlay
    if (omovlhisidx >= ARRAY_COUNT(omovlhis)) {
        return FALSE;
    }

    history = &omovlhis[++omovlhisidx];
    history->overlayID = ovlID;
    history->event = event;
    history->stat = stat;
    omOvlGotoEx(ovlID, event, stat);
    return TRUE;
}

RECOMP_PATCH void omOvlGotoEx(s32 ovlID, s16 event, u16 stat) {
    u8 var_a0_2;
    u8 var_v1;

    recomp_printf("omOvlGotoEx called\n");

    if (omovl != selmenu && omovl != selmenu2) {
        omprevovl = omovl;
        D_800A1756_A2356 = D_800CE202_CEE02;
    }
    
    D_800A176C_A236C = 1;
    omovl = ovlID;
    omovlevtno = event;
    omovlstat = stat;
    D_800D0A3A_D163A = 0;

    if (stat & 0x40 && D_800CCF52_CDB52 != 2) {
        func_8004A444_4B044(2);
    } else if (stat & 0x80 && (D_800CCF52_CDB52 != 3)) {
        func_8004A444_4B044(3);
    }

    if ((D_800D4082_D4C82 != D_800A1920_A2520[ovlID].unk_00) || (D_800CD2F4_CDEF4 != D_800A1920_A2520[ovlID].unk_02) || (D_800D6A56_D7656 != D_800A1920_A2520[ovlID].unk_04)) {
        func_8004A468_4B068(D_800A1920_A2520[ovlID].unk_00, D_800A1920_A2520[ovlID].unk_02, D_800A1920_A2520[ovlID].unk_04);
    }

    if (!(stat & 0x2000)) {
        if (stat & 2) {
            var_a0_2 = D_800A1740_A2340[GwSystem.current_board_index][0];
            var_v1 = D_800A1740_A2340[GwSystem.current_board_index][1];
        } else if (stat & 4) {
            var_a0_2 = D_800A16B0_A22B0[GwSystem.minigame_index - 1][0];
            var_v1 = D_800A16B0_A22B0[GwSystem.minigame_index - 1][1];
        } else {
            var_a0_2 = 1;
            var_v1 = 20;
        }

        if (stat & 0x100) {
            var_a0_2 = 1;
            var_v1 = 20;
        } else if (stat & 0x200) {
            var_a0_2 = 2;
            var_v1 = 20;
        } else if (stat & 0x400) {
            var_a0_2 = 3;
            var_v1 = 20;
        } else if (stat & 0x800) {
            var_a0_2 = 4;
            var_v1 = 20;
        } else if (stat & 0x1000) {
            var_a0_2 = 5;
            var_v1 = 20;
        }

        D_800A1784_A2384 = var_v1;
        D_800A1908_A2508 = var_a0_2;
    }
    D_800A190C_A250C = 0;
}

int dummyData1 = 1;
int dummyBss1;
