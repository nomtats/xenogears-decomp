import os
import sys

def convert_mdf_to_bin(mdf_path, bin_path):
    print(f"Converting {mdf_path} (2448-byte sectors) to {bin_path} (2352-byte sectors)...")
    sector_size_in = 2448
    sector_size_out = 2352

    if not os.path.exists(mdf_path):
        print(f"Error: {mdf_path} not found.")
        sys.exit(1)

    with open(mdf_path, "rb") as fin, open(bin_path, "wb") as fout:
        while True:
            data = fin.read(sector_size_in)
            if not data:
                break
            if len(data) >= sector_size_out:
                fout.write(data[:sector_size_out])
    
    print(f"Extraction of standard BIN track to {bin_path} complete.")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python3 strip_mdf.py <input.mdf> <output.bin>")
        sys.exit(1)
    convert_mdf_to_bin(sys.argv[1], sys.argv[2])
