import os
import re 
import sys
pswpath = sys.argv[1]
tmppath = sys.argv[2]
tmp_file = open(tmppath, "w")
f = open(pswpath, "r")
lines = f.readlines()
i=0
for line in lines:
    if line.find('>') > -1:
        i=i+1
        tmp_file.write(">"+str(i)+"\n")
        continue
    tmp_file.write(line)
f.close()
 
tmp_file.close()
