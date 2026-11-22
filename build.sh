#!/bin/sh
# Build with the pspdev toolchain (https://github.com/pspdev/pspdev).
# Output: bin/FusaGamePad/{EBOOT.PBP,usbgamepad.prx,config.ini}
set -e

root=$(cd "$(dirname "$0")" && pwd)
out="$root/bin/FusaGamePad"

make -C "$root/src/prx" clean
make -C "$root/src/prx"
make -C "$root/src/app" clean
make -C "$root/src/app"

mkdir -p "$out"
cp "$root/src/prx/usbgamepad.prx" "$out/"
cp "$root/src/app/EBOOT.PBP" "$out/"
cp "$root/src/app/config.ini" "$out/"

echo "Done. Copy $out to <memory stick>/PSP/GAME/"
