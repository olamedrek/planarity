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

parser = argparse.ArgumentParser()
parser.add_argument('-draw', action='store_true', required=False, default=False)
parser.add_argument('-input', action='store', required=True)
parser.add_argument('-exe', action='store', required=True)
args = parser.parse_args()

flag_draw = args.draw
in_path = args.input
exe_path = args.exe

out_path = in_path[:-2] + "out"
exp_path = in_path[:-2] + "exp"
testname = re.search('test[0-9]+', in_path).group(0)

subprocess.call([exe_path, in_path, out_path])

points = []
edges = []

with open(out_path) as output:
    biconnected = int(next(output)) == 1
    planar = int(next(output)) == 1

    if biconnected and planar:
        drawing_correct = int(next(output)) == 1

        for line in output:
            x, y = [int(i) for i in line.split()]
            points.append([x, y])

exp_present = os.path.isfile(exp_path)

if exp_present:
    with open(exp_path) as expected:
        exp_biconnected = int(next(expected)) == 1
        exp_planar = int(next(expected)) == 1

    result = True
    if exp_planar != planar or exp_biconnected != biconnected:
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

    if exp_present:
        if result:
            print "OK"
        else:
            print "WA"
    else:
        print "NA"

    sys.exit()


print "Biconnected:", biconnected
print "Planar:", planar
if planar and biconnected:
    print "Drawing correct:", drawing_correct
if exp_present:
    print "Result correct:", result
else:
    print "Result unknown"

if biconnected and planar:
    with open(in_path) as input:
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
        plt.annotate(i+1, (x[i], y[i]))

    if len(edges) > 0:
        plt.plot(x[edges.T], y[edges.T], linestyle='-', color='lightgray', markerfacecolor='black', marker='o')
    plt.axis('off')
    plt.show()
