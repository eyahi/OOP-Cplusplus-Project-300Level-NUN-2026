#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

PORT=8080

if [ ! -x "build/ChatServer" ]; then
  echo "ChatServer not built yet. Run: ./build_all.sh"
  exit 1
fi

# If something is already listening on the port, kill it (avoids 'address already in use').
PID="$(lsof -ti tcp:$PORT || true)"
if [ -n "$PID" ]; then
  echo "Port $PORT is busy (PID $PID). Killing it..."
  kill -9 $PID || true
fi

exec ./build/ChatServer
