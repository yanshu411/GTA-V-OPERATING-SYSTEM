/* Multiboot v1 x86 entry point, to be loaded by GRUB in 32-bit mode. */
.set MAGIC, 0x1BADB002
.set FLAGS, (1 << 0) | (1 << 1)
.set CHECKSUM, -(MAGIC + FLAGS)
.section .multiboot,"a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss,"aw",@nobits
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text,"ax"
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    push %ebx
    push %eax
    call kernel_main
.hang:
    cli
    hlt
    jmp .hang
