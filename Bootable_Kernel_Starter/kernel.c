/* ViceStreet OS: educational 32-bit x86 VGA + PS/2 keyboard kernel.
   THIS IS AN EARLY KERNEL PROTOTYPE, NOT A FULL OPERATING SYSTEM.
   Boots through GRUB (Multiboot v1) and runs a tiny shell. */
#include <stdint.h>
#include <stddef.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
static volatile uint16_t *const vga = (volatile uint16_t *)0xB8000;
static int row, col;
static uint8_t color = 0x0A; /* bright green on black */
static unsigned int cash = 12500;
static unsigned int contracts = 0;
static char command[128];
static unsigned int command_len;
static int shifted;

static inline unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
static inline void outb(unsigned short port, unsigned char value) {
    __asm__ volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}
static void cursor_update(void) {
    unsigned short pos = (unsigned short)(row * VGA_WIDTH + col);
    outb(0x3D4, 0x0F); outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E); outb(0x3D5, (pos >> 8) & 0xFF);
}
static void clear_screen(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        vga[i] = ((uint16_t)color << 8) | ' ';
    row = col = 0;
    cursor_update();
}
static void scroll(void) {
    if (row < VGA_HEIGHT) return;
    for (int y = 1; y < VGA_HEIGHT; ++y)
        for (int x = 0; x < VGA_WIDTH; ++x)
            vga[(y - 1) * VGA_WIDTH + x] = vga[y * VGA_WIDTH + x];
    for (int x = 0; x < VGA_WIDTH; ++x)
        vga[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((uint16_t)color << 8) | ' ';
    row = VGA_HEIGHT - 1;
}
static void putc(char ch) {
    if (ch == '\n') { col = 0; ++row; }
    else if (ch == '\r') { col = 0; }
    else if (ch == '\b') {
        if (col) --col;
        else if (row) { --row; col = VGA_WIDTH - 1; }
        vga[row * VGA_WIDTH + col] = ((uint16_t)color << 8) | ' ';
    } else {
        vga[row * VGA_WIDTH + col] = ((uint16_t)color << 8) | (uint8_t)ch;
        if (++col >= VGA_WIDTH) { col = 0; ++row; }
    }
    scroll(); cursor_update();
}
static void print(const char *s) { while (*s) putc(*s++); }
static int streq(const char *a, const char *b) {
    while (*a && *b && *a == *b) { ++a; ++b; }
    return *a == *b;
}
static void print_number(unsigned int n) {
    char digits[16]; int i = 0;
    do { digits[i++] = '0' + (n % 10); n /= 10; } while (n);
    while (i) putc(digits[--i]);
}
static void banner(void) {
    print("======================================================================\n");
    print("  V I C E S T R E E T   O S     //    L O S   S A N T O S\n");
    print("======================================================================\n");
    print("  CITY EDITION 0.1  |  Real x86 kernel starter  |  Fan-made concept\n");
    print("  VGA screen + PS/2 keyboard shell. Type HELP to explore.\n\n");
}
static void prompt(void) { print("player@los-santos:~$ "); }
static void execute(const char *cmd) {
    if (streq(cmd, "help")) {
        print("COMMANDS: help  about  city  map  garage  missions  cash\n");
        print("          clear  green  pink  reboot\n");
    } else if (streq(cmd, "about")) {
        print("ViceStreet OS 0.1 - a minimal bootable educational kernel.\n");
        print("This is not a Windows replacement or a GTA V game.\n");
    } else if (streq(cmd, "city")) {
        print("LOCATION: LOS SANTOS // STATUS: CITY ONLINE\n");
    } else if (streq(cmd, "map")) {
        print("           VINEWOOD\n");
        print("          /    |   \\ \n");
        print("     BEACH -- DOWNTOWN -- PORT\n");
        print("               |\n");
        print("             GARAGE\n");
    } else if (streq(cmd, "garage")) {
        print("LOS SANTOS CUSTOMS  |  CURRENT RIDE: COMET S\n");
    } else if (streq(cmd, "missions")) {
        if (!contracts) {
            contracts = 1; cash += 1250;
            print("FIRST DROP complete! Earned $1250 (simulated).\n");
        } else print("FIRST DROP already complete. More contracts soon.\n");
    } else if (streq(cmd, "cash")) {
        print("WALLET: $"); print_number(cash); putc('\n');
    } else if (streq(cmd, "clear")) {
        clear_screen(); banner();
    } else if (streq(cmd, "green")) {
        color = 0x0A; clear_screen(); banner();
    } else if (streq(cmd, "pink")) {
        color = 0x0D; clear_screen(); banner();
    } else if (streq(cmd, "reboot")) {
        print("Requesting keyboard-controller reset...\n");
        while (inb(0x64) & 0x02) { }
        outb(0x64, 0xFE);
    } else if (cmd[0]) print("Unknown command. Type help.\n");
}
/* PS/2 Set 1 scancodes (US layout, basic printable keys). */
static const char base_keys[128] = {
    [2]='1',[3]='2',[4]='3',[5]='4',[6]='5',[7]='6',[8]='7',[9]='8',[10]='9',[11]='0',
    [12]='-',[13]='=',[16]='q',[17]='w',[18]='e',[19]='r',[20]='t',[21]='y',[22]='u',[23]='i',[24]='o',[25]='p',
    [26]='[',[27]=']',[30]='a',[31]='s',[32]='d',[33]='f',[34]='g',[35]='h',[36]='j',[37]='k',[38]='l',[39]=';',
    [40]='\'',[41]='`',[43]='\\',[44]='z',[45]='x',[46]='c',[47]='v',[48]='b',[49]='n',[50]='m',[51]=',',[52]='.',[53]='/',[57]=' '
};
static const char shift_digits[12] = "!@#$%^&*()_+";
static void keyboard_loop(void) {
    for (;;) {
        if (!(inb(0x64) & 1)) continue;
        unsigned char scan = inb(0x60);
        if (scan == 42 || scan == 54) { shifted = 1; continue; }
        if (scan == (42 | 0x80) || scan == (54 | 0x80)) { shifted = 0; continue; }
        if (scan & 0x80) continue;
        if (scan == 28) {
            putc('\n'); command[command_len] = 0;
            execute(command); command_len = 0; prompt();
        } else if (scan == 14) {
            if (command_len) { --command_len; putc('\b'); }
        } else if (scan < 128 && base_keys[scan] && command_len < sizeof(command) - 1) {
            char ch = base_keys[scan];
            if (shifted && ch >= 'a' && ch <= 'z') ch -= 32;
            else if (shifted && ch >= '1' && ch <= '9') ch = shift_digits[ch - '1'];
            else if (shifted && ch == '0') ch = ')';
            /* Commands work case-insensitively by storing lowercase. */
            command[command_len++] = (ch >= 'A' && ch <= 'Z') ? ch + 32 : ch;
            putc(ch);
        }
    }
}
void kernel_main(uint32_t magic, uint32_t boot_info) {
    (void)boot_info;
    clear_screen();
    if (magic != 0x2BADB002) {
        print("ERROR: Bootloader did not provide a Multiboot entry.\n");
        for (;;) __asm__ volatile("hlt");
    }
    banner(); prompt(); keyboard_loop();
}
