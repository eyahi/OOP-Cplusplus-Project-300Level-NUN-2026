# Qt Music Player
MADE BY IFON ITOR 20233246
 JASON HIPPOLITE 20232884,
 PRINCE UMEH 20233270,
Qt Music Player is a simple desktop music player built with **Qt** and **C++**.  
It provides a clean interface for playing basic audio files using Qt Multimedia.

---

## Features

- Graphical interface built with Qt Widgets  
- Play, pause, stop, next and previous controls  
- Open a folder containing music files  
- Displays “Now Playing” track name  
- Progress bar and time display  
- Volume control slider  
- Keyboard-friendly and beginner-friendly project  

---

## Supported Audio Formats

This version **does NOT support MP3**.

Supported formats depend on the Qt Multimedia backend and system codecs.

Currently tested formats:

- WAV  
- OGG (on some systems)  
- Other uncompressed formats supported by Qt

❌ MP3 is **not supported** in this build.

If you try to open an MP3 file:
- It will not play  
- Time will stay at `0:00 / 0:00`  
- “Now Playing” may remain `(none)`

---

## Requirements

- Qt Creator  
- Qt 5/Qt 6 with Qt Multimedia  
- C++ compiler (MinGW or MSVC)  
- Windows OS  

---

## How to Build and Run

### Using Qt Creator

1. Open Qt Creator  
2. Click **Open Project**  
3. Open `QtMusicPlayer.pro` or `CMakeLists.txt`  
4. Configure your kit  
5. Click **Run**

---

## How to Use

1. Launch the app  
2. Click **Open Folder**  
3. Select a folder containing supported audio files (e.g. WAV)  
4. Select a song from the list  
5. Use:
   - Prev  
   - Play  
   - Stop  
   - Next  
6. Adjust volume with the slider  

---

## Standalone EXE

To create a standalone executable:

1. Build in **Release mode**  
2. Locate the `.exe` in your build folder  
3. Run:

```bash
windeployqt QtMusicPlayer.exe


