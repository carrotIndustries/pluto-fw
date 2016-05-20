#!/usr/bin/env python3
import sys

INVALID_COM = 3
DIG_OTHERS = 0xf

filename = sys.argv[1]

dig = None
named = None
seg = 0

tab = {}

with open(filename, "r") as fi :
	for li in fi.readlines() :
		li = li.strip()
		if len(li) == 0 :
			continue
		if li[0] == "#" :
			if li[1:] == "OTHERS" :
				dig = DIG_OTHERS
			else :
				dig=int(li[1:][-1])
			seg = 0
		elif li[0] == "-" :
			named=li[1:]
		else :
			if li == "x" :
				c,s = INVALID_COM, 0
			else :
				c,s = (int(x) for x in li.split())
			if s&1 : #odd
				tab[(dig<<4)|seg] = ((s//2)<<3)|(c+4)
			else : #even
				tab[(dig<<4)|seg] = ((s//2)<<3)|(c+0)
			seg += 1

print("#include <stdint.h>")
print("const uint8_t tgt_lcd_map[] = {")
for i in range(max(tab.keys())+1) :
	x = tab.get(i, INVALID_COM<<4)
	print(" 0x%.2x,"%x)
print("};")
