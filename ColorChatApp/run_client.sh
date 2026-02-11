#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

if [ ! -x "build/ColorChatClient" ]; then
  echo "ColorChatClient not built yet. Run: ./build_all.sh"
  exit 1
fi

./build/ColorChatClient
