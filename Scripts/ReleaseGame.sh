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

next_step "Package Windows version into zip file"
7z a -tzip -r \
  "$PROJECT_ROOT/Binaries/Package/HyperSlash-$VERSION-win-x64.zip" \
  "$PROJECT_ROOT/Binaries/Package/Windows/"*

next_step "Package Linux version into tar.gz file"
tar -czf "$PROJECT_ROOT/Binaries/Package/HyperSlash-$VERSION-linux-x64.tar.gz" \
  -C "$PROJECT_ROOT/Binaries/Package/Linux" .

next_step "Merge main into release"
git checkout -B release origin/release
git merge  --no-ff --no-edit origin/main
git push origin release

next_step "Create the tag"
git tag "$VERSION"
git push origin "$VERSION"

next_step "Create the GitHub release"
gh release create "$VERSION" \
  "$PROJECT_ROOT/Binaries/Package/HyperSlash-$VERSION"* \
  --target release \
  --title "$VERSION" \
  --generate-notes
