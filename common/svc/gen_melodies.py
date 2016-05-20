#!/usr/bin/env python3
import sys
import rtttl

timer_freq = int(sys.argv[2])

print('#include "melody.h"')

with open(sys.argv[1], "r") as mfile :
	lines = [line.strip() for line in mfile.readlines() if len(line.strip())]
	for i,line in enumerate(lines) :
		melody = rtttl.parse_rtttl(line)
		print("static const svc_melody_note_t mldy_%d[] = {"%i, end="")
		for note in melody["notes"] :
			print("{%d, %d},"%(round(note["frequency"]), note["duration"]/1000*timer_freq), end = " ")
		print("{0,0}", end="")
		print(" };\n")
		#print()
	print("const svc_melody_note_t *svc_melodies[] = {")
	for i in range(len(lines)) :
		print(" mldy_%d,"%i)
	print("};")
	print("const uint8_t svc_melodies_n = %d;\n"%len(lines));


