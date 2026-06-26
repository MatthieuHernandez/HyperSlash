#!/usr/bin/env bash
set -euo pipefail

UE_ROOT="/c/Program Files/Epic Games/UE_5.7"
PROJECT_ROOT="$(pwd)"
PROJECT_FILE="$PROJECT_ROOT/HyperSlash.uproject"
VERSION="${1:?Usage: ./build.sh <version>}"



echo "Downloading $VERSION version..."
curl -L "https://github.com/MatthieuHernandez/strudel-autoplay/releases/download/$VERSION/strudel-autoplay-$VERSION-win32-x64.zip" -o "./strudel-autoplay-$VERSION-win32-x64.zip"

echo "Removing existing directory..."
rm -rf "./strudel-autoplay"

echo "Extracting archive..."
unzip -q "./strudel-autoplay-$VERSION-win32-x64.zip" -d "./strudel-autoplay"

echo "Cleaning up..."
rm -rf "./strudel-autoplay-$VERSION-win32-x64.zip"