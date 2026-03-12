import os
import sys

if len(sys.argv) != 3:
    print("Usage: python3 rename_symbol.py <old> <new>")
    sys.exit(1)

old_name, new_name = sys.argv[1:3]

target_dirs = ["src", "include", "asm", "config", "yaml", "linker"]

for dir_name in target_dirs:
    for root, dirs, files in os.walk(dir_name):
        for file in files:
            if file.endswith(('.c', '.h', '.s', '.yaml', '.txt', '.ld')):
                path = os.path.join(root, file)
                try:
                    with open(path, "r") as f:
                        content = f.read()
                    
                    if old_name in content:
                        content = content.replace(old_name, new_name)
                        with open(path, "w") as f:
                            f.write(content)
                        print(f"Updated {path}")
                except Exception as e:
                    pass

print(f"Renamed {old_name} to {new_name}")
