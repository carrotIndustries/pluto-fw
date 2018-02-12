#!/usr/bin/env python3
import sys
import os

print("#pragma once")
print("#include <stdint.h>")
print('#include "common/svc/lcd.h"')

for mapfile in sys.argv[1:] :
	name = os.path.basename(mapfile).split(".")[0]
	print("extern const svc_lcd_map_t svc_lcd_map_%s;"%name)
