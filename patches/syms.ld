__start = 0x80000000;

/* Dummy addresses that get recompiled into function calls */
recomp_puts = 0x8F000000;
recomp_exit = 0x8F000004;
osPiReadIo_recomp = 0x8F000008;
setjmp_recomp = 0x8F00000C;
longjmp_recomp = 0x8F000010;
__divdi3_recomp = 0x8F000014;
__umoddi3_recomp = 0x8F000018;
__udivdi3_recomp = 0x8F00001C;