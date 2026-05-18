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

next_step "Release $VERSION"
mkdir -p "$PROJECT_ROOT/Builds"

next_step "Clear the folder"
rm -rf "$PROJECT_ROOT/Binaries/Package"

next_step "Merge main into release"
git checkout release
git merge main --squash
git push origin release

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
  -iostore \
  -archive \
  -archivedirectory="$PROJECT_ROOT/Binaries/Package/Linux" \
  -verbose

next_step "Package into archives"
(
  cd "$PROJECT_ROOT/Binaries/Package/Windows"
  zip -r "$PROJECT_ROOT/Builds/HyperSlash-$VERSION-win-x64.zip" .
)

cd "$PROJECT_ROOT/Binaries/Package/Linux"
tar -czf "$PROJECT_ROOT/Builds/HyperSlash-$VERSION-linux-x64.tar.gz" \
  -C "$PROJECT_ROOT/Binaries/Package/Linux" .

next_step "Test the game manually"
read -n 1 -s -r -p "Press any key to continue..."

next_step "Create the tag"
git tag "$VERSION"
git push origin "$VERSION"

next_step "Create the GitHub release"
gh release create "$VERSION" \
  "$PROJECT_ROOT"/Builds/HyperSlash-"$VERSION"* \
  --target release \
  --title "$VERSION" \
  --generate-notes
