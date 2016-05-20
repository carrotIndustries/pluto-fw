#!/usr/bin/env python3
import sys
import os

print("#include <stdint.h>")
print('#include "common/svc/lcd.h"')

for i in range(16) :
	print("#define SEG_%s (1<<%d)"%(chr(ord("A")+i), i))

maptype = 8

def idx_from_chr(c) :
	c = c.lower()
	o = ord(c)
	if c.isdigit() :
		return o - ord("0")
	elif ord("a") <= o <= ord("z") :
		return 10 + o - ord("a")
	elif c == "-" :
		return 36
	elif c == "/" :
		return 37

def mask_from_segs(s) :
	if len(s) == 0 :
		return "0"
	else :
		return "("+("|".join("SEG_"+x.upper() for x in s))+")"

mapfile = sys.argv[1]

with open(mapfile, "r") as fi :
	header = fi.readline().strip()
	if header == "MAP8" :
		maptype = 8
	elif header == "MAP16" :
		maptype = 16
	else :
		print("Invalid map type  %s"%header)
		exit(1)
	name = os.path.basename(mapfile).split(".")[0]
	
	print("static const uint%d_t map[] = {"%maptype)
	lines = (l.strip().split() for l in fi.readlines() if len(l.strip()))
	d = {idx_from_chr(k):v for k,v in lines}
	maxi = max(d.keys())+1
	for i in range(maxi) :
		s = d.get(i, "")
		print("", mask_from_segs(s)+",", "//%d"%i)
	print("};")
	print("const svc_lcd_map_t svc_lcd_map_%s = {"%name)
	print(".type = SVC_LCD_MAP_T_%d,"%maptype)
	print(".map = map,")
	print(".length = %d"%maxi)
	print("};")
