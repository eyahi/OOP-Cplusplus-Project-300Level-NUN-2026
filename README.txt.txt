Qt Music Player 🎵
Project Overview

Qt Music Player is a desktop music player application developed in C++ using Qt 6 (Widgets) for the graphical user interface and SFML for audio playback.

The application allows users to load a folder of music files, search through songs by title, artist, or lyrics (via external lyrics files), and control playback through a modern, user-friendly interface.

This project was built to demonstrate:

Object-oriented programming in C++

GUI development with Qt

File handling and error handling

Integration of third-party libraries (SFML)

Features

📂 Open and load a folder containing music files

▶️ Play / ⏸ Pause / ⏹ Stop playback

⏭ Next and ⏮ Previous track navigation

🔊 Volume control

⏱ Seek bar with time display

🔍 Search songs by Title, Artist, or Lyrics

🖱 Drag-and-drop files or folders into the app

📜 Right-click context menu (Play, Play Next, Reveal, Remove)

🖼 Album artwork support (cover.jpg/png or song-named image)

⚠️ Robust error handling (unsupported formats, missing files, playback failures)

Supported Audio Formats

The player supports the following formats (via SFML):

.wav

.ogg

.flac

.aiff

.au

❌ MP3 is NOT supported (SFML limitation in this project).

Lyrics Support

Lyrics are loaded from external sidecar files placed in the same folder as the song:

SongName.lrc

SongName.txt

Lyrics are searchable via the search bar.
(Embedded lyrics inside audio files are not used.)

Technologies Used

Language: C++ (C++17)

GUI Framework: Qt 6 (Widgets)

Audio Library: SFML (Audio & System)

Build System: CMake

Platform: Windows

Project Structure
QtMusicPlayer/
│
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── CMakeLists.txt
├── README.md


⚠️ Note: UI is built programmatically in C++ (Qt Designer .ui files are not used).

Build Instructions (Qt Creator – Recommended)
Requirements

Qt 6.x (Widgets module)

CMake ≥ 3.16

SFML (installed separately)

MinGW or compatible C++ compiler

Steps

Open Qt Creator

Select Open Project

Choose CMakeLists.txt

Configure the project with a Qt 6 Kit

If SFML is not auto-detected:

Set CMAKE_PREFIX_PATH to your SFML installation directory

Build and Run

How to Use

Launch the application

Click Open Folder or drag a folder into the window

Double-click a song to play

Use playback controls in the mini-player bar

Use the search bar to filter songs by title, artist, or lyrics

Error Handling

The application gracefully handles:

Unsupported audio formats

Empty folders

Missing or deleted files

Corrupted or unreadable audio files

Playback failures

Errors are shown as user-friendly warning dialogs, and the application does not crash.

Known Limitations

MP3 files are not supported

Metadata (artist/title) is inferred from filenames (e.g. Artist - Title.wav)

Lyrics must be provided as .lrc or .txt files

Author

[Itoro ifon, Jason Hippolite, Prince Umeh]
C++ / Qt Music Player Project

License

This project is for educational purposes.