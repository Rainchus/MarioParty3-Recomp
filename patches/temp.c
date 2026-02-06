#include "patches.h"
// #include "PR/os_message.h"

int dummyData = 1;
int dummyBss;

#include "ultra64.h"
#include "process_funcs.h"

struct om_obj_data;
typedef void (*omObjFunc)(struct om_obj_data*);

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

typedef struct om_obj_data {
    /* 0x00 */ u16 stat;
    /* 0x02 */ s16 next_idx_alloc;
    /* 0x04 */ s16 prio;
    /* 0x06 */ s16 prev;
    /* 0x08 */ s16 next;
    /* 0x0A */ s16 next_idx;
    /* 0x0C */ s16 group;
    /* 0x0E */ u16 group_idx;
    /* 0x10 */ u32 unk10;
    /* 0x14 */ omObjFunc func;
    /* 0x18 */ Vec trans;
    /* 0x24 */ Vec rot;
    /* 0x30 */ Vec scale;
    /* 0x3C */ u16 mdlcnt;
    /* 0x40 */ s16* model;
    /* 0x44 */ u16 mtncnt;
    /* 0x48 */ s16* motion;
    /* 0x4C */ u32 work[4];
    /* 0x5C */ void* data;
} omObjData;

typedef struct OvlEntrypoint {
    /* 0x00 */ s16 index;
    /* 0x04 */ void (*fn)(void);
} OvlEntrypoint; // sizeof 0x08

typedef struct RGB {
    /* 0x00 */ u8 r;
    /* 0x01 */ u8 g;
    /* 0x02 */ u8 b;
} RGB; // sizeof 0x3

typedef struct str800D5298 {
    s32 unk0;
    s32 unk4;
    void* unk8;  // mbf0 rom
    void* unkC;  // sbf0 rom
    void* unk10; // fxd0 rom
    void* unk14; // audio ram location?
    s32 unk18;   // audio ram size?
    s32 unks1C24[3];
    s32 unk28;
    s32 unk2C;
    s32 unks3040[5];
    s32 unk44;
    s32 unks4858[5];
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
} str800D5298;

typedef struct HeapNode {
    /* 0x00 */ s32 size;
    /* 0x04 */ u8 heap_constant;
    /* 0x05 */ u8 active;
    /* 0x06 */ char pad[2];
    /* 0x08 */ struct HeapNode* prev;
    /* 0x0C */ struct HeapNode* next;
} HeapNode;

typedef struct UnkOvl81 {
    /* 0x00 */ void* unk0;
    /* 0x04 */ void* unk4;
    /* 0x08 */ void* unk8;
    /* 0x0C */ char unk_0C[0x14];
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ char unk_22[2];
} UnkOvl81;

typedef struct UnkOvl81_1 {
    s32 unk_00;
    char unk_04[0xC];
} UnkOvl81_1;

typedef struct Unk800D6B48 {
    void* unk_00;
    void* unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s16 unk_18;
} Unk800D6B48; // sizeof 0x18

typedef struct Unk3 {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    omObjData* unk_04;
    s16 winID;
    char unk_0A[2];
    char* unk_0C; // messageID / pointer to message?
} Unk3;           // sizeof 0x10

typedef struct {
    /* 0x00 */ char unk00[2];
    /* 0x02 */ s16 unk02;
    /* 0x04 */ char unk04[0x14];
} D_800CCF58_CDB58_Struct; // Size 0x18

//typedef struct jump_buf
//{
//    u32* sp;
//    void *func;
//    u32 regs[21];
//} jmp_buf;
//
//extern s32 setjmp(jmp_buf *jump_buf);
//extern s32 longjmp(jmp_buf *jump_buf, s32 val);

#define EXEC_PROCESS_DEFAULT 0
#define EXEC_PROCESS_SLEEPING 1
#define EXEC_PROCESS_WATCH 2
#define EXEC_PROCESS_DEAD 3

typedef void (*process_func)();

typedef struct Process {
/* 0x00 */ struct Process *next;
/* 0x04 */ struct Process *youngest_child;
/* 0x08 */ struct Process *oldest_child;
/* 0x0C */ struct Process *relative;
/* 0x10 */ struct Process *parent_oldest_child;
/* 0x14 */ struct Process *new_process;
/* 0x18 */ void *heap;
/* 0x1C */ u16 exec_mode;
/* 0x1E */ u16 stat;
/* 0x20 */ u16 priority;
/* 0x22 */ s16 dtor_idx;
/* 0x24 */ s32 sleep_time;
/* 0x28 */ void *base_sp;
// /* 0x2C */ jmp_buf prc_jump;
/* 0x2C */ s32 coro_created;
/* 0x30 */ s32 yield_value;
/* 0x34 */ s32 coro_func;
/* 0x38 */ s32 mips_sp;
/* 0x3C */ char unk_3C[0x48]; //part of jmp_buf but we dont need it anymore. pad the struct out so the other offsets are the same
/* 0x84 */ s32 dtor_idxCopy;
/* 0x88 */ process_func destructor;
/* 0x8C */ void *user_data;
} Process;

void HuPrcSysInit();
Process* HuPrcCreate(process_func func, u16 priority, s32 stack_size, s32 extra_data_size);
void HuPrcChildLink(Process*process, Process*child);
void HuPrcChildUnlink(Process*process);
Process* HuPrcCreateChild(process_func func, u16 priority, s32 stack_size, s32 extra_data_size, Process* parent);
void HuPrcChildWait();
Process* HuPrcCurrentGet();
s32 HuPrcChildGet(Process*process);
s32 HuPrcStatKill(Process*process);
void HuPrcKill(Process*process);
void HuPrcChildKill(Process*process);
void HuPrcTerminate(Process*process);
void HuPrcExit();
void HuPrcSleep(s32 time);
void HuPrcVSleep();
void HuPrcAwake(Process*process);
void HuPrcDtor(Process*process, process_func destructor);
void HuPrcCurrentDtor(process_func destructor);
void* HuPrcAllocMem(s32 size);
void HuPrcFreeMem(void *ptr);


void* HuMemHeapInit(void* ptr, u32 size);
void* HuMemMemoryAlloc(HeapNode* heap, s32 size);
void HuMemMemoryFree(void* ptr);
void* HuMemMemoryRealloc(HeapNode* heap, void* mem, u32 new_size);
u32 HuMemUsedMemorySizeGet(HeapNode* heap);
u32 HuMemUsedMemoryBlockGet(HeapNode* heap);
s32 HuMemMemoryAllocSizeGet(s32 value);

HeapNode* HuMemHeapInitPerm(void* ptr, u32 size);
void* HuMemMemoryAllocPerm(u32 size);
void HuMemMemoryFreePerm(void* ptr);
void* HuMemMemoryReallocPerm(void* mem, u32 new_size);
u32 HuMemHeapAllocPermSizeGet(void);
u32 HuMemUsedMemoryBlockGetPerm(void);

HeapNode* HuMemHeapInitTemp(void* ptr, u32 size);
void* HuMemMemoryAllocTemp(u32 size);
void HuMemMemoryFreeTemp(void* ptr);
void* HuMemMemoryReallocTemp(void* mem, u32 new_size);
u32 HuMemHeapAllocTempSizeGet(void);
u32 HuMemUsedMemoryBlockGetTemp(void);
//#include "malloc.h"

// Forward declarations for coroutine system
// These should match your minicoro wrapper functions
extern void recomp_process_init(void);
// extern void recomp_process_coro_create(u32 id, void *func, u32 stack_size, u32 mips_sp);
extern void recomp_process_coro_destroy(u32 id);
extern s32 recomp_process_switch_to(u32 id, s32 resume_value);
extern s32 recomp_process_yield(s32 reason);

extern Process* top_process;
extern Process* current_process;
extern s16 process_count;

// pointer to where HuPrcVSleep was called
extern u32 sPrcSleepLoc;

// Global process ID counter for coroutine identification
static u32 next_process_id = 1;

// At the top of your file, after the static next_process_id declaration:
#define MAX_PROCESSES 256  // Adjust based on your game's needs
static u8 process_id_used[MAX_PROCESSES] = {0};

static u32 allocate_process_id(void) {
    for (u32 i = 1; i < MAX_PROCESSES; i++) {
        if (!process_id_used[i]) {
            process_id_used[i] = 1;
            return i;
        }
    }
    // No free IDs! This is bad
    return 0;
}

static void free_process_id(u32 id) {
    if (id > 0 && id < MAX_PROCESSES) {
        process_id_used[id] = 0;
    }
}

// ============================================================================
// Helper: Yield to scheduler with a reason
// ============================================================================
static s32 yield_to_scheduler(s32 reason) {
    if (current_process) {
        current_process->yield_value = reason;
    }
    return recomp_process_yield(reason);
}

// pending heap to free after a process terminates itself
// the scheduler sets this before yielding and checks it after
static void* g_pending_heap_free = NULL;

RECOMP_PATCH void HuPrcSysInit()
{
    // Initialize coroutine system first
    recomp_process_init();
    
    process_count = 0;
    top_process = NULL;
    current_process = NULL;
    next_process_id = 1;
}

void LinkProcess(Process** root, Process* process)
{
    Process* src_process = *root;
    if (src_process != NULL && (src_process->priority >= process->priority))
    {
        while (src_process->next != NULL)
        {
            if (src_process->next->priority < process->priority)
            {
                break;
            }
            src_process = src_process->next;
        }

        process->next = src_process->next;
        process->youngest_child = src_process;
        src_process->next = process;
        if (process->next)
        {
            process->next->youngest_child = process;
        }
    }
    else
    {
        process->next = (*root);
        process->youngest_child = NULL;
        *root = process;
        if (src_process != NULL)
        {
            src_process->youngest_child = process;
        }
    }
}

void UnlinkProcess(Process** root, Process* process)
{
    if (process->next)
    {
        process->next->youngest_child = process->youngest_child;
    }
    if (process->youngest_child)
    {
        process->youngest_child->next = process->next;
    }
    else
    {
        *root = process->next;
    }
}

RECOMP_PATCH Process* HuPrcCreate(process_func func, u16 priority, s32 stack_size, s32 extra_data_size) {
    HeapNode* process_heap;
    Process* process;
    s32 alloc_size;
    void* stack_mem;

    if (stack_size == 0) {
        stack_size = 2048;
    }

    alloc_size = HuMemMemoryAllocSizeGet(sizeof(Process)) + HuMemMemoryAllocSizeGet(stack_size) +
                 HuMemMemoryAllocSizeGet(extra_data_size);

    process_heap = (HeapNode*) HuMemMemoryAllocPerm(alloc_size);
    if (process_heap == NULL) {
        return NULL;
    }
    HuMemHeapInit(process_heap, alloc_size);

    process = (Process*) HuMemMemoryAlloc(process_heap, sizeof(Process));
    process->heap = process_heap;
    process->exec_mode = EXEC_PROCESS_DEFAULT;
    process->stat = 0;
    process->priority = priority;
    process->sleep_time = 0;

    // Allocate stack ONCE
    stack_mem = HuMemMemoryAlloc(process_heap, stack_size);
    process->base_sp = stack_mem + stack_size - 8;

    // Initialize coroutine fields instead of jmp_buf
    process->coro_created = 0;
    process->yield_value = 0;
    process->coro_func = (s32)func;
    process->mips_sp = stack_size;  // Store stack size
    process->dtor_idxCopy = allocate_process_id();  // Use recycled IDs
    
    if (process->dtor_idxCopy == 0) {
        // Out of process IDs!
        HuMemMemoryFreePerm(process_heap);
        return NULL;
    }

    process->destructor = NULL;
    process->user_data = NULL;
    LinkProcess(&top_process, process);
    process->oldest_child = NULL;
    process->relative = NULL;
    process_count++;
    return process;
}

RECOMP_PATCH void HuPrcChildLink(Process* process, Process* child)
{
    HuPrcChildUnlink(child);
    if (process->oldest_child)
    {
        process->oldest_child->new_process = child;
    }
    child->parent_oldest_child = process->oldest_child;
    child->new_process = NULL;
    process->oldest_child = child;
    child->relative = process;
}

RECOMP_PATCH void HuPrcChildUnlink(Process* process)
{
    if (process->relative)
    {
        if (process->parent_oldest_child)
        {
            process->parent_oldest_child->new_process = process->new_process;
        }
        if (process->new_process)
        {
            process->new_process->parent_oldest_child = process->parent_oldest_child;
        }
        else
        {
            process->relative->oldest_child = process->parent_oldest_child;
        }
        process->relative = NULL;
    }
}

RECOMP_PATCH Process* HuPrcCreateChild(process_func func, u16 priority, s32 stack_size, s32 extra_data_size, Process* parent)
{
    Process* child = HuPrcCreate(func, priority, stack_size, extra_data_size);
    HuPrcChildLink(parent, child);
    return child;
}

RECOMP_PATCH void HuPrcChildWait()
{
    Process* process = HuPrcCurrentGet();
    if (process->oldest_child)
    {
        process->exec_mode = EXEC_PROCESS_WATCH;
        yield_to_scheduler(YIELD_NORMAL);
    }
}

RECOMP_PATCH Process* HuPrcCurrentGet()
{
    return current_process;
}

RECOMP_PATCH s32 HuPrcChildGet(Process* process)
{
    s32 i;
    Process* curr_child = process->oldest_child;
    i = 0;
    while (curr_child)
    {
        curr_child = curr_child->parent_oldest_child;
        i++;
    }
    return i;
}

RECOMP_PATCH s32 HuPrcStatKill(Process* process)
{
    if (process->exec_mode != EXEC_PROCESS_DEAD)
    {
        HuPrcAwake(process);
        process->exec_mode = EXEC_PROCESS_DEAD;
        return 0;
    }
    else
    {
        return -1;
    }
}

RECOMP_PATCH void HuPrcKill(Process* process)
{
    HuPrcChildKill(process);
    HuPrcChildUnlink(process);
    HuPrcStatKill(process);
}

RECOMP_PATCH void HuPrcChildKill(Process* process)
{
    Process* curr_child = process->oldest_child;
    while (curr_child != NULL)
    {
        if (curr_child->oldest_child != NULL) {
            HuPrcChildKill(curr_child);
        }
        HuPrcStatKill(curr_child);
        curr_child = curr_child->parent_oldest_child;
    }
    process->oldest_child = NULL;
}

RECOMP_PATCH void HuPrcTerminate(Process* process)
{
    if (process->destructor)
    {
        process->destructor();
    }
    
    // save heap pointer before we do anything else.
    // the process struct lives inside this heap, so we have to
    // read everything we need before freeing
    void* heap_to_free = process->heap;
    u32 process_id = process->dtor_idxCopy;
  
    UnlinkProcess(&top_process, process);
    process_count--;

    // store heap pointer in global so scheduler can free it
    // we can't store it in the process struct because that's inside the heap
    g_pending_heap_free = heap_to_free;
    
    // Free the process ID for reuse
    free_process_id(process_id);
    
    // Yield back to scheduler. the coroutine will be in DEAD state
    // after this, and the scheduler will destroy it + free the heap
    yield_to_scheduler(YIELD_TERMINATE);
}

RECOMP_PATCH void HuPrcExit()
{
    Process* process = HuPrcCurrentGet();
    HuPrcChildKill(process);
    HuPrcChildUnlink(process);
    HuPrcTerminate(process);
}

RECOMP_PATCH void HuPrcSleep(s32 time)
{
    Process* process = HuPrcCurrentGet();
    if (time != 0 && process->exec_mode != EXEC_PROCESS_DEAD)
    {
        process->exec_mode = EXEC_PROCESS_SLEEPING;
        process->sleep_time = time;
    }
    yield_to_scheduler(YIELD_NORMAL);
}

RECOMP_PATCH void HuPrcVSleep()
{
    HuPrcSleep(0);
}

RECOMP_PATCH void HuPrcAwake(Process* process)
{
    process->sleep_time = 0;
}

RECOMP_PATCH void HuPrcDtor(Process* process, process_func destructor)
{
    process->destructor = destructor;
}

RECOMP_PATCH void HuPrcCurrentDtor(process_func destructor)
{
    Process* process = HuPrcCurrentGet();
    HuPrcDtor(process, destructor);
}

RECOMP_PATCH void HuPrcCall(s32 time)
{
    Process* cur_proc_local;
    Process* next_proc;
    s32 yield_reason;

    current_process = top_process;

    while (1) {
        cur_proc_local = current_process;
        if (cur_proc_local == NULL) {
            break;
        }

        // Update sPrcSleepLoc if needed
        sPrcSleepLoc = 0;

        // Check stat flag
        if (cur_proc_local->stat & 0x1) {
            if (cur_proc_local->exec_mode != 3) {
                current_process = current_process->next;
                continue;
            }
        }

        // Handle different execution modes
        switch (cur_proc_local->exec_mode) {
            case EXEC_PROCESS_SLEEPING:
                if (cur_proc_local->sleep_time > 0 && (cur_proc_local->sleep_time -= time) <= 0) {
                    cur_proc_local->sleep_time = 0;
                    cur_proc_local->exec_mode = EXEC_PROCESS_DEFAULT;
                }
                current_process = current_process->next;
                break;

            case EXEC_PROCESS_WATCH:
                if (cur_proc_local->oldest_child != 0) {
                    current_process = current_process->next;
                } else {
                    cur_proc_local->exec_mode = EXEC_PROCESS_DEFAULT;
                    // Fall through to run the process
                    goto run_process;
                }
                break;

            case EXEC_PROCESS_DEAD:
                // Process is dead, need to run HuPrcExit on it
                // Destroy old coroutine first if it exists
                if (cur_proc_local->coro_created) {
                    recomp_process_coro_destroy(cur_proc_local->dtor_idxCopy);
                    cur_proc_local->coro_created = 0;
                }

                cur_proc_local->coro_func = (s32) HuPrcExit;
                // Fall through to run exit
            case EXEC_PROCESS_DEFAULT:
            run_process:
                // Create coroutine on first run
                if (!cur_proc_local->coro_created) {
                    // mips_sp field stores the stack size
                    u32 stack_size = cur_proc_local->mips_sp;
                    // Calculate actual MIPS SP (top of stack)
                    u32 mips_sp = (u32) cur_proc_local->base_sp + 8;

                    recomp_process_coro_create(cur_proc_local->dtor_idxCopy, cur_proc_local->coro_func, stack_size,
                                               mips_sp);
                    cur_proc_local->coro_created = 1;
                }

                // Save next pointer BEFORE switching, because the process
                // may terminate itself and free its memory
                next_proc = cur_proc_local->next;

                // Clear pending free
                g_pending_heap_free = NULL;

                // Switch to this process and wait for it to yield
                yield_reason = recomp_process_switch_to(cur_proc_local->dtor_idxCopy, 1);

                // Handle yield reasons
                if (yield_reason == YIELD_TERMINATE) {
                    // Process terminated itself
                    recomp_process_coro_destroy(cur_proc_local->dtor_idxCopy);

                    // Free the heap that the process stored for us
                    if (g_pending_heap_free != NULL) {
                        HuMemMemoryFreePerm(g_pending_heap_free);
                        g_pending_heap_free = NULL;
                    }

                    // Use saved next pointer since process is now freed
                    current_process = next_proc;
                } else {
                    // YIELD_NORMAL
                    current_process = cur_proc_local->next;
                }
                break;
        }
    }
}


RECOMP_PATCH void* HuPrcAllocMem(s32 size)
{
    Process* process = HuPrcCurrentGet();
    recomp_printf("HuPrcAllocMem: size=%d, current_process=%p\n", size, process);
    
    if (process == NULL) {
        recomp_printf("HuPrcAllocMem: ERROR - current_process is NULL!\n");
        return NULL;
    }
    
    recomp_printf("HuPrcAllocMem: process->heap=%p, process->dtor_idxCopy=%d\n", 
           process->heap, process->dtor_idxCopy);
    
    if (process->heap == NULL) {
        recomp_printf("HuPrcAllocMem: ERROR - process->heap is NULL for process %d!\n", 
               process->dtor_idxCopy);
        return NULL;
    }
    
    void* result = (void *)HuMemMemoryAlloc((HeapNode *)process->heap, size);
    recomp_printf("HuPrcAllocMem: Allocated %p from heap %p\n", result, process->heap);
    return result;
}

RECOMP_PATCH void HuPrcFreeMem(void *ptr)
{
    HuMemMemoryFree(ptr);
}

s32 dmaStart(OSIoMesg * msg, u8 pri, s32 direction, u8* src, u8* dest, u32 size, OSMesgQueue * retQueue);
extern OSMesgQueue D_800B29F0_B35F0;

typedef struct OverlayTable {
/* 0x00 */ u8* romStart;
/* 0x04 */ u8* romEnd;
/* 0x08 */ u8* vramStart;
/* 0x0C */ u8* textVramStart;
/* 0x10 */ u8* textVramEnd;
/* 0x14 */ u8* dataVramStart;
/* 0x18 */ u8* dataVramEnd;
/* 0x1C */ u8* bssVramStart;
/* 0x20 */ u8* bssVramEnd;
} OverlayTable; //sizeof 0x24

typedef struct RectF {
/* 0x00 */ f32 x1;
/* 0x04 */ f32 y1;
/* 0x08 */ f32 x2;
/* 0x0C */ f32 y2;
} RectF; //sizeof 0x10

#define HAS_TEXT_SECTION 1
#define HAS_DATA_SECTION 2
#define HAS_BSS_SECTION 4

extern OverlayTable _ovltbl[];
extern OverlayTable _modeovltbl[];
s32 dmaReadOvl(u8* src, u8* dest, s32 size);
extern u8 D_800962F0_96EF0;
extern u32 rnd_seed;
extern RectF D_800975F0_981F0[];
extern Vec D_80097560_98160[];
extern Vec D_800975A8_981A8[];
void recomp_load_overlays(u8* rom, u8* ram_addr, unsigned int size);
// void unload_overlays(void* ram_addr, unsigned int size);

typedef struct OvlData {
    u8* curOverlayVRamAddr;
    u32 curOverlaySize;
} OvlData;

OvlData loadOverlayDataMain = {
    .curOverlayVRamAddr = 0,
    .curOverlaySize = 0,
};

OvlData loadOverlayDataSecondary = {
    .curOverlayVRamAddr = 0,
    .curOverlaySize = 0,
};

RECOMP_PATCH void func_8000B1A0_BDA0(s32 arg0, u8 sectionFlags) {
    u8* textVramEnd;
    u8* textVramStart;
    u8* romStart;
    u8* dataVramStart;
    u8* dataVramEnd;
    u8* bss_end;
    u8* bss_start;
    u8* curBssAddr;
    u8* textStart;
    u8* tmp;
    OvlData* ovlData;

    romStart = _modeovltbl[arg0].romStart;
    textVramStart = _modeovltbl[arg0].textVramStart;
    textVramEnd = _modeovltbl[arg0].textVramEnd;
    dataVramStart = _modeovltbl[arg0].dataVramStart;
    dataVramEnd = _modeovltbl[arg0].dataVramEnd;
    bss_start = _modeovltbl[arg0].bssVramStart;
    bss_end = _modeovltbl[arg0].bssVramEnd;
    textStart = _modeovltbl[arg0].vramStart;

    if (textVramStart == 0x801059A0) {
        ovlData = &loadOverlayDataMain;
    } else {
        ovlData = &loadOverlayDataSecondary;
    }

    // if (ovlData->curOverlayVRamAddr != 0 && ovlData->curOverlaySize != 0) {
    //     unload_overlays((void*)ovlData->curOverlayVRamAddr, ovlData->curOverlaySize);
    // }

    ovlData->curOverlayVRamAddr = textVramStart;
    ovlData->curOverlaySize = dataVramEnd - textVramStart;   

    recomp_load_overlays(romStart, ovlData->curOverlayVRamAddr, ovlData->curOverlaySize);

    if (sectionFlags & HAS_TEXT_SECTION) {
        dmaReadOvl(romStart, textStart, textVramEnd - textVramStart);
    }
    if (sectionFlags & HAS_DATA_SECTION) {
        dmaReadOvl(romStart + (textVramEnd - textVramStart), dataVramStart, dataVramEnd - dataVramStart);
    }
    if (sectionFlags & HAS_BSS_SECTION) {
        curBssAddr = tmp = bss_start;
        while (tmp < bss_end) {
            *curBssAddr++ = 0;
            tmp++;
        }
    }
}

// copies in an overlay and clears bss region.
RECOMP_PATCH void OvlLoad(s32 overlayIndex) {
    u8* rom_start;
    u8* rom_end;
    u8* bss_start;
    u8* bss_end;
    u8* curBssAddr;
    OvlData* ovlData;

    rom_start = _ovltbl[overlayIndex].romStart;
    rom_end = _ovltbl[overlayIndex].romEnd;
    bss_start = _ovltbl[overlayIndex].bssVramStart;
    bss_end = _ovltbl[overlayIndex].bssVramEnd;

    if (_ovltbl[overlayIndex].vramStart == 0x801059A0) {
        ovlData = &loadOverlayDataMain;
    } else {
        ovlData = &loadOverlayDataSecondary;
    }

    // if (ovlData->curOverlayVRamAddr != 0 && ovlData->curOverlaySize != 0) {
    //     unload_overlays((void*)ovlData->curOverlayVRamAddr, ovlData->curOverlaySize);
    // }

    ovlData->curOverlayVRamAddr = _ovltbl[overlayIndex].vramStart;
    ovlData->curOverlaySize = rom_end - rom_start;   

    recomp_load_overlays(rom_start, ovlData->curOverlayVRamAddr, ovlData->curOverlaySize);
    dmaReadOvl(rom_start, _ovltbl[overlayIndex].vramStart, rom_end - rom_start);

    curBssAddr = bss_start;
    while (bss_start < bss_end) {
        *curBssAddr++ = 0;
        bss_start++;
    }

    D_800962F0_96EF0 = 0;
}

extern HeapNode* perm_heap_addr;

// Perm Heap created at FFFFFFFF80140000, size: 001A0000
// Temp Heap created at FFFFFFFF80128000, size: 00018000

RECOMP_PATCH HeapNode* HuMemHeapInitPerm(void* ptr, u32 size) {
    bzero(ptr, size);
    perm_heap_addr = (HeapNode*) HuMemHeapInit(ptr, size);
    recomp_printf("Perm Heap created at %08lX, size: %08X\n", perm_heap_addr, size);
    return perm_heap_addr;
}

extern HeapNode* temp_heap_addr;

/*
 * Creates the temporary heap.
 */
RECOMP_PATCH HeapNode* HuMemHeapInitTemp(void* ptr, u32 size) {
    bzero(ptr, size);
    temp_heap_addr = (HeapNode*) HuMemHeapInit(ptr, size);
    recomp_printf("Temp Heap created at %08lX, size: %08X\n", temp_heap_addr, size);
    return temp_heap_addr;
}

#define ALIGN_16(size) (((size) + 0x1F) & ~0xF)
#define HEAP_CONSTANT 0xA5
#define MIN_ALLOC_SIZE 16

#define MIN_HEAP_NODE_SIZE (sizeof(HeapNode) + MIN_ALLOC_SIZE)

extern s32 D_800A0530_A1130;
extern Gfx* gMainGfxPos;

RECOMP_PATCH void* HuMemMemoryAlloc(HeapNode* heap, s32 size)
{
    HeapNode* cur_heap;
    HeapNode* new_heap_temp;

    size = ALIGN_16(size);

    cur_heap = heap;

    do
    {

        if (!cur_heap->active)
        {
            if (cur_heap->size >= size)
            {
                if ((u32)(cur_heap->size - size) > MIN_HEAP_NODE_SIZE)
                {
                    new_heap_temp = ((void *)cur_heap) + size;
                    new_heap_temp->size = cur_heap->size - size;
                    new_heap_temp->heap_constant = HEAP_CONSTANT;
                    new_heap_temp->active = FALSE;

                    cur_heap->next->prev = new_heap_temp;
                    new_heap_temp->next = cur_heap->next;
                    cur_heap->next = new_heap_temp;
                    new_heap_temp->prev = cur_heap;
                    cur_heap->size = size;
                }

                cur_heap->active = TRUE;
                return (void *)cur_heap + sizeof(HeapNode);
            }
        }

        cur_heap = cur_heap->next;
    }
    while (cur_heap != heap);

    D_800A0530_A1130 = 0x10000;
    return NULL;
}

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

typedef struct omOvlHisData { //Object Manager History Data
/* 0x00 */ s32 overlayID;
/* 0x04 */ s16 event;
/* 0x06 */ s16 stat;
} omOvlHisData; //sizeof 0x08

extern omOvlHisData omovlhis[12];
extern s16 omovlhisidx;
void omOvlGotoEx(s32 ovlID, s16 event, u16 stat);

RECOMP_PATCH s32 omOvlCallEx(s32 ovlID, s16 event, u16 stat) {
    omOvlHisData* history;

    // history is full, cannot call overlay
    if (omovlhisidx >= ARRAY_COUNT(omovlhis)) {
        return FALSE;
    }

    // if (ovlID == 0x7A) {
    //     ovlID = 0x7F; //if loading intro, load debug overlay
    // }

    history = &omovlhis[++omovlhisidx];
    history->overlayID = ovlID;
    history->event = event;
    history->stat = stat;
    omOvlGotoEx(ovlID, event, stat);
    return TRUE;
}

RECOMP_PATCH void* HuMemMemoryRealloc(HeapNode *heap, void *mem, u32 new_size)
{
    void *ret;
    HeapNode *given_heap;
    HeapNode *new_heap;
    s32 temp;

    static int realloc_counter = 0;
    realloc_counter++;
    recomp_printf("=== REALLOC #%d: mem=%p new_size=%d ===", realloc_counter, mem, new_size);
    
    given_heap = (HeapNode *)((char*)mem - sizeof(HeapNode)); // FIX: explicit char* cast
    temp = ALIGN_16(new_size);
    
    if (given_heap->size >= temp)
    {
        if ((u32)(given_heap->size - temp) > MIN_HEAP_NODE_SIZE)
        {
            new_heap = (HeapNode*)((char*)given_heap + temp); // FIX: char* arithmetic
            new_heap->size = given_heap->size - temp;
            new_heap->heap_constant = HEAP_CONSTANT;
            new_heap->active = FALSE;
            given_heap->next->prev = new_heap;
            new_heap->next = given_heap->next;
            given_heap->next = new_heap;
            new_heap->prev = given_heap;
            given_heap->size = temp;
        }
        return (void*)((char*)given_heap + sizeof(HeapNode)); // FIX: char* arithmetic
    }
    else
    {
        ret = HuMemMemoryAlloc(heap, new_size);
        if (ret != NULL)
        {
            // The size field contains just the data size (without header)
        {
            char* dst = (char*)ret;
            char* src = (char*)mem;
            s32 count = given_heap->size;
            while (count--) {
                *dst++ = *src++;
            }
        }
            HuMemMemoryFree(mem);
        }
        return ret;
    }
    
    return NULL;
}

extern u32 D_800A1200_A1E00[];
extern u32 D_800A11F0_A1DF0[];
extern s32 D_800A1210_A1E10[];
extern u32 D_800A1220_A1E20[];
extern s32 D_800A1230_A1E30[];

// RECOMP_PATCH void func_8003465C_3525C(Gfx** gfxPtr, u32 texAddr, s32 fmt, s32 siz, s32 width, s32 height, s32 scaleS, s32 scaleT, s32 texWidth, s32 texHeight, s32 cm_s, s32 mask_s, s32 shift_s, s32 cm_t, s32 mask_t, s32 shift_t, s32 pal) {
//     Gfx* gfx;
//     u32 lineWidth;
//     u32 lineSize;
//     u32 t;
//     u32 dxt;

//     recomp_printf("gfxPtr is %08lX\n", gfxPtr);
    
//     // gfx = *gfxPtr;
//     // texAddr += (width * scaleT) << (siz - 1);
    
//     // gDPSetTextureImage(gfx++, fmt, D_800A1210_A1E10[siz], 1, texAddr);
//     // gDPSetTile(gfx++, fmt, D_800A1210_A1E10[siz], 0, 0, G_TX_LOADTILE, 0, cm_t, mask_t, shift_t, cm_s, mask_s, shift_s);
//     // gDPLoadSync(gfx++);
    
//     // lineWidth = (width * D_800A11F0_A1DF0[siz]) >> 3;
//     // lineSize = lineWidth ? (lineWidth + 0x7FF) : 0x800;
    
//     // t = ((((width * height) + D_800A1220_A1E20[siz]) >> D_800A1230_A1E30[siz]) - 1);
//     // if (t >= 0x800) {
//     //     t = 0x7FF;
//     // }
    
//     // gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, t, lineSize / lineWidth);
//     gDPPipeSync(gfx++);
    
//     // gDPSetTile(gfx++, fmt, siz, (width * D_800A1200_A1E00[siz] + 7) >> 3, 0, G_TX_RENDERTILE, pal, cm_t, mask_t, shift_t, cm_s, mask_s, shift_s);
//     // gDPSetTileSize(gfx++, G_TX_RENDERTILE, texWidth * 4, scaleS * 4, texHeight * 4, scaleT * 4);
    
//     *gfxPtr = gfx;
// }

extern void* D_800CCF38_CDB38;

void func_8000BA30_C630(void);
void func_80014A3C_1563C(s32);
void func_8001B0B4_1BCB4(void**, s32);
u8 rand8(void);

void HmfLightInit(void);                                   /* extern */
void* HuMemAlloc(s32);                                /* extern */
void HuMemInit(void);                                      /* extern */
void func_80030040_30C40(void);                            /* extern */
void func_80031560_32160(void);                            /* extern */
extern u8 D_800A0AD0_A16D0[];
extern u16 D_800C9528_CA128;
extern s8 D_800CB8B0_CC4B0;
extern s8 D_800CC0A8_CCCA8;
extern s16 D_800CC0B6_CCCB6;
extern void* D_800CC0BC_CCCBC;
extern void* D_800CD1B8_CDDB8[];
extern u8 D_800CDA84_CE684;
extern s8 D_800CE1C6_CEDC6;
extern s32 D_800CE1D4_CEDD4;
extern u8 D_800D0599_D1199;
extern s8 D_800D10F0_D1CF0;
extern void* D_800D1F38_D2B38[];
extern s16 D_800D1FEE_D2BEE;
extern u8 D_800D1FF0_D2BF0;
extern void* D_800D2070_D2C70[];
extern u8 D_800D41C4_D4DC4;
extern f32 D_800D51FC_D5DFC;
extern f32 D_800D5410_D6010;
extern void* D_800D6A98_D7698[];
extern s16 D_800D6B64_D7764;

extern u8 D_800D2008_D2C08;
extern u16 D_800D5306_D5F06;

RECOMP_PATCH void func_800222B0_22EB0(void** arg0, s32 arg1, u16 arg2, u16 arg3, u16 arg4, u8 arg5) {
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s1;
    s16 var_s1_2;
    s32 temp_s0;
    s16 i;

    D_800D1FF0_D2BF0 = arg5;
    D_800D2008_D2C08 = 0;
    HuMemInit();
    D_800D1FEE_D2BEE = arg2;
    D_800C9528_CA128 = arg4;
    D_800D5306_D5F06 = arg3;

    for (i = 0; i < arg5; i++) {
        D_800D6A98_D7698[i] = HuMemAlloc(arg2 * 16);
        D_800CD1B8_CDDB8[i] = HuMemAlloc(arg4 << 6);
        D_800D1F38_D2B38[i] = HuMemAlloc(arg3 * 8);
    }
    
    for (i = 0; i < 8; i++) {
        D_800D2070_D2C70[i] = HuMemAlloc(D_800A0AD0_A16D0[i] * 8);
    }

    D_800CC0BC_CCCBC = HuMemAlloc(0x8400);
    D_800CE1D4_CEDD4 = 0;
    HmfLightInit();
    func_80031560_32160();
    func_80030040_30C40();
    D_800CB8B0_CC4B0 = 1;
    D_800D10F0_D1CF0 = 2;
    D_800D5410_D6010 = 1.0f;
    D_800D51FC_D5DFC = 1.0f;
    D_800CE1C6_CEDC6 = 0;
    D_800CDA84_CE684 = -1;
    D_800D41C4_D4DC4 = -1;
    D_800D0599_D1199 = -1;
    D_800D6B64_D7764 = 0x3B6;
    D_800CC0B6_CCCB6 = 0x3E8;
    D_800CC0A8_CCCA8 = 1;
}