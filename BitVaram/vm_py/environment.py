import json
import sys
from symbol_table import SymbolTable

CORE_PATH = "./environment.bv"

try:
    # initializing BitVaram core definitions
    with open(CORE_PATH, 'r') as f:
        core = json.load(f)

except OSError as e:
    print("failed to initialize BitVaram's core", file=sys.stderr)
    sys.exit(1)

GLOBAL_DEFINES = SymbolTable(core)

def update_core():
    try:
        core = GLOBAL_DEFINES.core  # TODO: choose only global definition
        # saving BitVaram core definitions
        with open(CORE_PATH, 'w') as f:
            json.dump(core, f)

    except OSError as e:
        print(f"failed to update BitVaram's core: {e}", file=sys.stderr)
        sys.exit(1)
