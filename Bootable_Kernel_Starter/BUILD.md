# ViceStreet OS 0.1 — bootable experimental kernel

**This is an educational 32-bit x86 kernel project.** Unlike the Chrome desktop demo, the kernel can boot under GRUB in an emulator. It only supports the text VGA display and a basic PS/2 keyboard shell. It **does not** have a graphical desktop, multitasking, memory management, persistent files, networking, an installer or production hardware drivers. Do not install it on your actual PC.

## On Windows 10/11

Use Ubuntu in **WSL2** or an Ubuntu VM (WSL itself cannot directly run this kernel, but QEMU launches a virtual machine).

In Ubuntu, run:

```bash
sudo apt update
sudo apt install -y build-essential binutils grub-pc-bin grub-common xorriso qemu-system-x86
cd /path/to/Bootable_Kernel_Starter
make
make iso
make run
```

The `make iso` command creates `ViceStreet_OS.iso` and `make run` boots it in QEMU. On WSL without Linux GUI support, use a desktop-capable VM or configure a QEMU display backend. You can also boot the ISO using VirtualBox.

## Try these shell commands

`help`, `about`, `city`, `map`, `garage`, `missions`, `cash`, `clear`, `green`, `pink`, `reboot`.

This is a separate, minimal proof-of-concept kernel. The richer `ViceStreet_OS.html` is a browser desktop UI concept and is **not running on the kernel**.

## Disclaimer

Unofficial game-inspired fan concept using original content. Not affiliated with Rockstar Games.
