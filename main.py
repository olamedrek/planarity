#!/usr/bin/python

import sys
import subprocess
import os
import matplotlib.pyplot as plt
import matplotlib
import numpy

font = {'size' : 14}
matplotlib.rc('font', **font)

executable = sys.argv[1]
testname = sys.argv[2]
input_path = 'test/' + testname + '.in'
output_path = 'test/' + testname + '.out'
expected_path = 'test/' + testname + '.exp'

subprocess.call([os.getcwd() + '/' + executable, input_path, output_path])

points = []
edges = []

with open(input_path) as input:
    n, m = [int(i) for i in next(input).split()]
    for line in input:
        u, v = [int(i)-1 for i in line.split()]
        edges.append([u, v])

with open(output_path) as output:
    result = int(next(output)) == 1
    if result:
        for line in output:
            x, y = [int(i) for i in line.split()]
            points.append([x, y])
    else:
        print 'NOT PLANAR'
        sys.exit()

if os.path.isfile(expected_path):
    with open(expected_path) as expected:
        expected_result = int(next(expected)) == 1
        if expected_result != result:
            print 'ERROR'
            sys.exit()

points = numpy.array(points)
edges = numpy.array(edges)

x = points[:,0].flatten()
y = points[:,1].flatten()

for i in range(n):
    plt.annotate(i+1, (x[i] + 0.1, y[i] + 0.1))

plt.plot(x[edges.T], y[edges.T], linestyle='-', color='lightgray', markerfacecolor='black', marker='o')
plt.axis('off')
plt.show()


