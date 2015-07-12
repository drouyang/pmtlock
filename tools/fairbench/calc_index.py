# Author: Jianna Ouyang
#!/usr/bin/python
"""
INPUT:
a
b
c
d
e
f
g
...

Caculate fairness index of every N values
"""
import sys
import multiprocessing


N = multiprocessing.cpu_count()
if N > 8:
	N = 8
i = 0

sum = 0
sum_of_square = 0

for line in sys.stdin:
	n = float(line)
	i += 1

	sum += n
	sum_of_square += n*n
	
	if (i % N == 0):
		print (sum*sum) / (N*sum_of_square)
		sum = 0
		sum_of_square = 0
