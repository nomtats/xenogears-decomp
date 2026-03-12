#!/bin/bash
set -e

echo "=== Automated Asset Extraction Setup ==="

DISC_DIR="disc"

echo "[1/5] Cleaning previous extraction artifacts..."
cd $DISC_DIR
rm -f SLUS_006.64 SYSTEM.CNF *.LZSS.BIN BATTLE.BIN BATTLING.BIN FIELD.BIN MENU.BIN MOVIE.BIN WORLD_MAP.BIN xenogears-iat.iso Xenogears.bin

echo "[2/5] Checking for RAW Disc Images..."
if [ -f "Xenogears-disc1.mdf" ]; then
    echo "Found raw MDF structure. Stripping subchannel data (2448 -> 2352 bytes)..."
    python3 ../tools/scripts/strip_mdf.py Xenogears-disc1.mdf Xenogears.bin
elif [ ! -f "Xenogears.bin" ]; then
    echo "ERROR: Neither Xenogears-disc1.mdf nor Xenogears.bin found in disc/ folder."
    exit 1
fi

echo "[3/5] Bypassing CD-ROM XA formatting with iat to extract SLUS_006.64..."
iat Xenogears.bin xenogears-iat.iso > /dev/null

echo "[4/5] Ripping ISO 9660 contents using 7zip..."
7z x xenogears-iat.iso > /dev/null
rm xenogears-iat.iso

echo "[5/5] Bypassing filesystem to carve custom LZSS overlays..."
python3 ../tools/scripts/extract_overlays.py

echo "=== Success! ==="
ls -l SLUS_006.64 FIELD.BIN MOVIE.BIN
