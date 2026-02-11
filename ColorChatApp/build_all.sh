#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

rm -rf build

echo "Configuring with Qt from Homebrew…"
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"

echo "Building…"
cmake --build build -j

echo ""
echo "Done. Binaries:"
echo "  - $ROOT/build/ChatServer"
echo "  - $ROOT/build/ColorChatClient"
