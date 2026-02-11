#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

# Ensure built
if [ ! -x "build/ChatServer" ] || [ ! -x "build/ColorChatClient" ]; then
  echo "Binaries missing. Run: ./build_all.sh"
  exit 1
fi

# Start server in background (kills port if needed)
./run_server.sh &
SERVER_PID=$!

# Give it a moment
sleep 0.4

# Start two clients so you can demo sending + receiving + online users
./build/ColorChatClient &
sleep 0.2
./build/ColorChatClient &

echo "Demo running. If you want to stop the background server: kill $SERVER_PID"
