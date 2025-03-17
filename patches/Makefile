TARGET = patches.elf

CC      ?= clang
LD      ?= ld.lld

CFLAGS   := -target mips -mips2 -mabi=32 -O2 -G0 -mno-abicalls -mno-odd-spreg -mno-check-zero-division \
			-fomit-frame-pointer -ffast-math -fno-unsafe-math-optimizations -fno-builtin-memset \
			-Wall -Wextra -Wno-incompatible-library-redeclaration -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable -Wno-missing-braces -Wno-unsupported-floating-point-opt
CPPFLAGS := -nostdinc -D_LANGUAGE_C -DMIPS -I dummy_headers -I ../lib/dk64_decomp/include -I ../lib/dk64_decomp/include/2.0L/PR -I ../lib/dk64_decomp/include/2.0L -I ../lib/dk64_decomp/include/libc -I ../lib/dk64_decomp/src -I../lib/rt64/include
LDFLAGS  := -nostdlib -T patches.ld -T syms.ld -Map patches.map --unresolved-symbols=ignore-all --emit-relocs
ASFLAGS   := -target mips -mips2 -mabi=32 -O2 -G0 -mno-abicalls -mno-odd-spreg -mno-check-zero-division


# Find all C source files and assembly source files
C_SRCS    := $(wildcard *.c)
S_SRCS := $(wildcard *.s)

# Create object lists for both C and assembly files
C_OBJS    := $(C_SRCS:.c=.o)
S_OBJS := $(S_SRCS:.s=.o)

# Dependency files for C sources
C_DEPS := $(C_SRCS:.c=.d)
S_DEPS := $(S_SRCS:.s=.d)

# Link target includes both C and assembly objects
$(TARGET): $(C_OBJS) $(S_OBJS) patches.ld syms.ld
	$(LD) $(C_OBJS) $(ASM_OBJS) $(LDFLAGS) -o $@

# Rule for compiling C files
$(C_OBJS): %.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

# Rule for assembling .s files
$(S_OBJS): %.o : %.s
	$(CC) $(ASFLAGS) $(CPPFLAGS) -x assembler-with-cpp $< -MMD -MF $(@:.o=.d) -c -o $@

clean:
	rm -rf $(C_OBJS) $(S_OBJS) $(TARGET) $(C_DEPS) $(S_DEPS)

-include $(C_DEPS)
-include $(S_DEPS)

.PHONY: clean
