import os

file = "cuore.txt"

with open(file,"r") as f:
	lines = f.readlines()

sep = 10

x0 = int(float(lines[0][0:sep-1]))
y0 = int(float(lines[0][sep:]))


for i in range(1,len(lines)):
    x = int(float(lines[i][0:sep-1]))
    y = int(float(lines[i][sep:-1]))
    
    print(x-x0,",",y-y0,",")
    
    x0 = x
    y0 = y

