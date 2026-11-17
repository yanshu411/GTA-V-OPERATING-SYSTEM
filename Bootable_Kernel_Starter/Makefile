CC ?= gcc
LD ?= ld
CFLAGS := -m32 -ffreestanding -O2 -Wall -Wextra -fno-pic -fno-pie -fno-stack-protector -fno-builtin
all: vicestreet.bin
boot.o: boot.S
	$(CC) -m32 -c $< -o $@
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@
vicestreet.bin: boot.o kernel.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $@ boot.o kernel.o
iso: vicestreet.bin
	mkdir -p iso/boot/grub
	cp vicestreet.bin iso/boot/vicestreet.bin
	cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o ViceStreet_OS.iso iso
run: iso
	qemu-system-i386 -cdrom ViceStreet_OS.iso -m 128M
clean:
	rm -rf boot.o kernel.o vicestreet.bin ViceStreet_OS.iso iso
.PHONY: all iso run clean
