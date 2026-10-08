#!/usr/bin/env bash
set -euo pipefail
: "${AP_HOME:?Set AP_HOME to async-profiler installation}"
PID="${1:?Usage: ./profiler.sh <PID>}"
"$AP_HOME/profiler.sh" -e cpu -d 30 -f execcore-cpu.html "$PID"
