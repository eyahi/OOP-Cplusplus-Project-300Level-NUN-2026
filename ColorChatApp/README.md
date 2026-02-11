# ColorChatApp (Qt)

A **Qt/C++ real-time chat application** consisting of:
- **ChatServer** (console, Qt Network)
- **ColorChatClient** (Qt Widgets GUI client)

## Authors
- **Chigozirim Glory Igwe** (242030137)
- **Daniel Ali Ameh** (20232446)
- **Abdulrahman Mohammed** (20231571)

## Features
- Global room (auto-join on registration)
- Private rooms (stable room ID for the same 2 users)
- Group rooms (multi-user chat)
- Colorful message bubbles + dark/neon UI theme
- Online users list (server sends snapshot of already-connected users)
- Reply-to message support (protocol field)
- Newline-delimited JSON protocol (safe message framing over TCP)

## Requirements
- **CMake 3.20+**
- A **C++17** compiler
- **Qt 6 recommended** (Qt 5 fallback supported)

## Build

### Cross-platform (recommended)

```bash
cd ColorChatApp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### macOS (Homebrew Qt)

```bash
brew install qt cmake
cd ColorChatApp
chmod +x build_all.sh run_demo.sh run_server.sh run_client.sh
./build_all.sh
```

> `build_all.sh` uses `brew --prefix qt` to locate Qt.

### Linux (Debian/Ubuntu example)

```bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base-dev qt6-base-dev-tools
cd ColorChatApp
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### Optional: warnings + AddressSanitizer

Warnings are ON by default. To enable ASAN (Debug recommended):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build -j
```

## Run

Open **two terminals**:

### Terminal 1: Start the server

```bash
./build/ChatServer
```

### Terminal 2: Start the GUI client

```bash
./build/ColorChatClient
```

Then connect to:
- **Host:** `127.0.0.1`
- **Port:** `8080`

### Quick demo (optional)

Runs the server + opens two GUI clients:

```bash
./run_demo.sh
```

## Sample usage
1. Launch **ChatServer**
2. Launch **ColorChatClient**
3. Enter host/port and a nickname, then connect
4. Start chatting in the global room or create a DM/group room

## limitations / issues
- This uses an in-memory room/message store (no database persistence).
- If a client closes abruptly, messages are not persisted after server restart.

## Dependencies
- Qt Core, Qt Network, Qt Widgets
