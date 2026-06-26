#!/usr/bin/env bash
set -euo pipefail

UE_ROOT="/c/Program Files/Epic Games/UE_5.7"
PROJECT_ROOT="$(pwd)"
PROJECT_FILE="$PROJECT_ROOT/HyperSlash.uproject"
VERSION="${1:?Usage: ./build.sh <version>}"



echo "Downloading $VERSION version..."
curl -L "https://github.com/MatthieuHernandez/strudel-autoplay/releases/download/$VERSION/strudel-autoplay-$VERSION-linux-x86_64.zip" -o "./strudel-autoplay-$VERSION-linux-x86_64.zip"
curl -L "https://github.com/MatthieuHernandez/strudel-autoplay/releases/download/$VERSION/strudel-autoplay-$VERSION-win32-x64.zip" -o "./strudel-autoplay-$VERSION-win32-x64.zip"

echo "Extracting archive..."
unzip -Coq "./strudel-autoplay-$VERSION-win32-x64.zip" -d "./Content/strudel-autoplay/tmp1"
unzip -Coq "./strudel-autoplay-$VERSION-linux-x86_64.zip" -d "./Content/strudel-autoplay/tmp2"

cp ./Content/strudel-autoplay/tmp1/strudel-autoplay ./Content/strudel-autoplay/strudel-autoplay.exe
cp ./Content/strudel-autoplay/tmp2/strudel-autoplay ./Content/strudel-autoplay/strudel-autoplay-linux

echo "Cleaning up..."
rm -rf ./Content/strudel-autoplay/tmp*
rm -rf ./*.zip