#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}"

LVGL_TEMPLATE="${PROJECT_ROOT}/lvgl/lv_conf_template.h"
DEFAULTS_PATH="${PROJECT_ROOT}/lv_conf.defaults"
OUTPUT_PATH="${PROJECT_ROOT}/lv_conf.h"
PY_SCRIPT="${PROJECT_ROOT}/lvgl/scripts/generate_lv_conf.py"

if [ ! -f "$DEFAULTS_PATH" ]; then
    echo "Config file not found: $DEFAULTS_PATH"
    exit 1
fi

echo "➡ Using config: $DEFAULTS_PATH"

python3 "$PY_SCRIPT" \
    --template "$LVGL_TEMPLATE" \
    --defaults "$DEFAULTS_PATH" \
    --config "$OUTPUT_PATH"

echo "lv_conf.h generated at: $OUTPUT_PATH"
