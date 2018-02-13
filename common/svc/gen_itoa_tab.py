#!/usr/bin/env python3
import sys

n = 100

if len(sys.argv)==2 :
    if sys.argv[1] == 'h' :
        print("#pragma once\nextern const char svc_itoa_tab [%d][2];"%n)
    else :
        print("const char svc_itoa_tab [%d][2] = {"%n)
        for i in range(100) :
            print('"%02d",'%i)
        print("};")
else :
    raise ValueError("Give one argument!")
