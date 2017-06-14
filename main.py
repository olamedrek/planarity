#!/usr/bin/python

import sys
import subprocess
import os
import matplotlib.pyplot as plt
import matplotlib
import numpy
import re
import argparse

font = {'size' : 14}
matplotlib.rc('font', **font)

executable = "cmake-build-debug/planarity"

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('-draw', action='store_true', required=False, default=False)
parser.add_argument('-path', action='store', required=True)
args = parser.parse_args()

flag_draw = args.draw
input_path = args.path

output_path = input_path[:-2] + "out"
expected_path = input_path[:-2] + "exp"
testname = re.search('test[0-9]+', input_path).group(0)

subprocess.call([os.getcwd() + '/' + executable, input_path, output_path])

points = []
edges = []

with open(output_path) as output:
    biconnected = int(next(output)) == 1
    planar = int(next(output)) == 1

    if biconnected and planar:
        drawing_correct = int(next(output)) == 1

        for line in output:
            x, y = [int(i) for i in line.split()]
            points.append([x, y])

expected_present = os.path.isfile(expected_path)

if expected_present:
    with open(expected_path) as expected:
        expected_biconnected = int(next(expected)) == 1
        expected_planar = int(next(expected)) == 1

    result = True
    if expected_planar != planar or expected_biconnected != biconnected:
        result = False


if flag_draw == False:
    print testname,

    if planar and biconnected:
        if drawing_correct:
            print "OK",
        else:
            print "WA",
    else:
        print "NA",

    if expected_present:
        if result:
            print "OK"
        else:
            print "WA"
    else:
        print "NA"

    sys.exit()


print "biconnected", biconnected
print "planar", planar
if planar and biconnected:
    print "drawing correct", drawing_correct
if expected_present:
    print "result", result
else:
    print "result Unknown"

if biconnected and planar:
    with open(input_path) as input:
        n, m = [int(i) for i in next(input).split()]
        cnt = 0
        for line in input:
            u, v = [int(i)-1 for i in line.split()]
            edges.append([u, v])
            cnt += 1
            if cnt == m:
                break

    points = numpy.array(points)
    edges = numpy.array(edges)

    x = points[:,0].flatten()
    y = points[:,1].flatten()

    for i in range(n):
        plt.annotate(i+1, (x[i] + 0.1, y[i] + 0.1))

    plt.plot(x[edges.T], y[edges.T], linestyle='-', color='lightgray', markerfacecolor='black', marker='o')
    plt.axis('off')
    plt.show()


