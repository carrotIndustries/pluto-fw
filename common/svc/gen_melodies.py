#!/usr/bin/env python3
import sys
import rtttl

timer_freq = int(sys.argv[2])

print('#include "melody.h"')

titles = []
with open(sys.argv[1], "r") as mfile :
	lines = [line.strip() for line in mfile.readlines() if len(line.strip())]
	for i,line in enumerate(lines) :
		melody = rtttl.parse_rtttl(line)
		titles += [melody['title']]
		print("static const svc_melody_note_t mldy_%d[] = {"%i, end="")
		for note in melody["notes"] :
			print("{%d, %d},"%(round(note["frequency"]), note["duration"]/1000*timer_freq), end = " ")
		print("{0,0}", end="")
		print(" };\n")
		#print()
	print("const svc_melody_t svc_melodies[] = {")
	for i in range(len(lines)) :
		print("{\"%s\", mldy_%d}%s"%(titles[i],i, (',' if i<(len(lines)-1) else '')))
	print("};")
	print("const uint8_t svc_melodies_n = %d;\n"%len(lines));
