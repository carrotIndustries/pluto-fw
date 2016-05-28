#!/usr/bin/env python3

n = 60
print("const char svc_itoa_tab [%d][2] = {"%n)
for i in range(60) :
	print('"%02d",'%i)
print("};")
