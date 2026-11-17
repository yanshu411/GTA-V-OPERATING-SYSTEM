# ViceStreet OS 2 — GTA V × Windows Desktop

**ViceStreet OS 2** is an unofficial GTA V–inspired Windows-like desktop **simulator**, made with a single HTML file, vanilla CSS and JavaScript. It launches directly in Chrome without npm, installers, or external dependencies.

> **Important:** This is not Microsoft Windows or a bootable operating system. It cannot execute `.exe` programs, read arbitrary folders on your computer, run Windows services, or install actual Windows applications. Built-in apps run safely within the browser. An independent, experimental x86 kernel starter is included in `Bootable_Kernel_Starter/` for learning OS development, but it does **not** run the graphical desktop.

## Launch

1. Download `index.html` from GitHub (or clone the repository).
2. Open `index.html` in **Chrome** or Edge. No npm/server required.
3. Explore the Start menu, search built-in apps, or double-click desktop shortcuts.

## Built-in desktop apps

| App | What works |
| --- | --- |
| File Explorer | Create, rename, edit, delete, import/export text-based files; use Recycle Bin |
| Notepad | Edit and save text documents, download files, Ctrl+S |
| Calculator | Arithmetic, parentheses, clear/backspace, keyboard input |
| Paint | Draw with mouse/touch, color/brush controls, export PNG |
| Photos | View local images selected through a file picker; show the themed wallpaper |
| Media Player | Play local audio/video in formats supported by your browser |
| Clock | Live clock, stopwatch, and countdown timer |
| Calendar | Browse months, save and edit simple events locally |
| Camera | Optionally use your webcam with explicit browser permission, save snapshots |
| Task Manager | Inspect and close windows created inside ViceStreet OS |
| Recycle Bin | Restore deleted text files or permanently remove them |
| Web Browser | Fictional offline City Web plus an address field for opening external sites in a new Chrome tab |
| Terminal | Original simulated terminal commands |
| Settings | Accent colors, city wallpapers, restore defaults |
| Missions, Garage, City Map, Phone, Sticky Notes, Radio | GTA-style original fictional activities |

## Controls

- Start menu: click **V** on taskbar or `Ctrl + Space`.
- Terminal: `Ctrl + Alt + T`.
- Notepad: `Ctrl + S` saves while editing.
- Double-click the title bar to maximize a window; drag the title bar to move windows.
- Close, maximize and minimize buttons work, with taskbar indicators.
- Information such as text files, missions, calendar events, virtual money, settings, and notes saves using `localStorage` in the browser. It is limited by browser storage and can be cleared by the browser.
- Imports use explicit file-selection permission; they cannot access folders or run software automatically.

## Project layout

```text
index.html                       # Windows-like browser desktop, all CSS & JS included
README.md                        # Documentation
Bootable_Kernel_Starter/         # Separate educational x86 kernel starter
    boot.S
    kernel.c
    linker.ld
    grub.cfg
    Makefile
    BUILD.md
screenshots/
    desktop.png
    apps.png
    start-menu.png
```

## Engineering notes

- Uses no frameworks, no npm and no CDN.
- Text content is HTML-escaped before being rendered into the UI.
- Audio and videos load from user-selected local files; they are not uploaded.
- Photo/media object URLs are revoked when apps close.
- Canvas drawings export through browser download API.
- Calculator uses its own small math expression parser rather than running entered JavaScript.
- Website URLs open in a normal new browser tab since most websites forbid embedding in iframes.

## Future development

A real Windows-compatible operating system requires kernel development, hardware drivers, virtual memory, networking, a filesystem, a window compositor, executable loading and broad application compatibility. This Chrome prototype can serve as a UI design and interaction prototype, but it does not provide these OS services.

## License and attribution

This is an **unofficial, fan-made, educational concept** inspired by GTA V / Los Santos. It is **not affiliated with or endorsed by Rockstar Games**. No GTA game assets are included. The city artwork, interface and simulated content are original.