#!/bin/bash

set -e

# Step 1: Remove and recreate build directory
echo "[INFO] Cleaning build directory..."
rm -rf build64
mkdir build64
cd build64

# Step 2: Run cmake and make with max cores
echo "[INFO] Running CMake..."
cmake .. -G "Unix Makefiles"

echo "[INFO] Building with make..."
make -j"$(nproc)"

# Step 3: Find container exposing port 8004
echo "[INFO] Searching for Docker container exposing port 8004..."
CONTAINER_ID=$(docker ps --filter "publish=8004" --format "{{.ID}}" | head -n 1)

if [ -z "$CONTAINER_ID" ]; then
    echo "[ERROR] No running Docker container found exposing port 8004"
    exit 1
fi

echo "[INFO] Found container: $CONTAINER_ID"

# Step 4: Copy the built plugin to /plugins inside container
PLUGIN_FILE="src/discord04rel64.so"
if [ ! -f "$PLUGIN_FILE" ]; then
    echo "[ERROR] Plugin file '$PLUGIN_FILE' not found in build64"
    exit 1
fi

echo "[INFO] Copying $PLUGIN_FILE into container:/plugins ..."
docker cp "$PLUGIN_FILE" "$CONTAINER_ID":/plugins/

echo "[SUCCESS] Plugin deployed successfully."
