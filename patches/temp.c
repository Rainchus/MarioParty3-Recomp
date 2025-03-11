#include "patches.h"
#include "PR/os_message.h"

typedef enum pointertable_e {
    TABLE_00_MIDI,
    TABLE_01_MAP_GEOMETRY,
    TABLE_02_MAP_WALLS,
    TABLE_03_MAP_FLOORS,
    TABLE_04_PROP_GEOMETRY,
    TABLE_05_ACTOR_GEOMETRY,
    TABLE_06_UNUSED,
    TABLE_07_TEXTURES_UNCOMPRESSED,
    TABLE_08_CUTSCENES,
    TABLE_09_SETUP,
    TABLE_10_SCRIPTS,
    TABLE_11_ANIMATIONS,
    TABLE_12_TEXT,
    TABLE_13_ANIM_CODE,
    TABLE_14_TEXTURES_HUD,
    TABLE_15_PATHS,
    TABLE_16_SPAWNERS,
    TABLE_17_DKTV,
    TABLE_18_TRIGGERS,
    TABLE_19_UNKNOWN,
    TABLE_20_UNKNOWN,
    TABLE_21_AUTOWALKS,
    TABLE_22_CRITTERS,
    TABLE_23_EXITS,
    TABLE_24_CHECKPOINTS,
    TABLE_25_TEXTURES_GEOMETRY,
    TABLE_26_UNCOMPRESSED_SIZES,
    TABLE_27_UNUSED,
    TABLE_28_UNUSED,
    TABLE_29_UNUSED,
    TABLE_30_UNUSED,
    TABLE_31_UNUSED
} pointertable_e;

typedef struct AutowalkFile {
    s16 count;
    s16 data[];
} AutowalkFile;

void *getPointerTableFile(enum pointertable_e pointerTableIndex, s32 fileIndex, u8 arg2, u8 arg3);
void func_global_asm_806F4528(AutowalkFile *);
void func_global_asm_8066B434(void *arg0, s32 arg1, s32 arg2);

RECOMP_PATCH void func_global_asm_806F3760(s16 map) {
    AutowalkFile *autowalkFile;

    autowalkFile = getPointerTableFile(TABLE_21_AUTOWALKS, map, 1, 0);

    recomp_printf("Autowalk File Loaded\n");
    recomp_printf("test count: %08X\n", autowalkFile->count);

    func_global_asm_806F4528(autowalkFile);
    if (autowalkFile) {
        func_global_asm_8066B434(autowalkFile, 0x4C, 0x56);
    }
}