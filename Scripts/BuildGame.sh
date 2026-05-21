#!/usr/bin/env bash
set -euo pipefail

UE_ROOT="/c/Program Files/Epic Games/UE_5.7"
PROJECT_ROOT="$(pwd)"
PROJECT_FILE="$PROJECT_ROOT/HyperSlash.uproject"
VERSION="${1:?Usage: ./build.sh <version>}"

STEP=0

next_step() {
  echo "============================================================="
  echo "========================= $STEP) $1 "
  echo "============================================================="
  STEP=$((STEP + 1))
}
next_step "Build $VERSION"

next_step "Clear the folder"
rm -rf "$PROJECT_ROOT/Binaries/Package"

next_step "Build the game for Windows"
"$UE_ROOT/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun \
  -project="$PROJECT_FILE" \
  -noP4 \
  -platform=Win64 \
  -clientconfig=Shipping \
  -build \
  -cook \
  -stage \
  -pak \
  -iostore \
  -archive \
  -archivedirectory="$PROJECT_ROOT/Binaries/Package/Windows" \
  -verbose

next_step "Build the game for Linux"
"$UE_ROOT/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun \
  -project="$PROJECT_FILE" \
  -noP4 \
  -platform=Linux \
  -clientconfig=Shipping \
  -build \
  -cook \
  -stage \
  -pak \
  -archive \
  -archivedirectory="$PROJECT_ROOT/Binaries/Package/Linux" \
  -unattended \
  -stdout \
  -verbose
