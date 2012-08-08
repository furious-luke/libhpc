#!/usr/bin/env python

import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def render_frame(blocks, filename, xaxis):
    plt.clf()

    x = blocks.keys()
    y = [blocks[v] - v for v in x]

    plt.bar(x, len(x)*[1.0], y)
    #plt.axis([xaxis[0], xaxis[1], 0, 2])
    plt.draw()
    plt.savefig(filename)

if __name__ == '__main__':

    blocks = {}

    if len(sys.argv) != 2:
        print 'Wrong'
        sys.exit(1)

    lower = None
    upper = None

    for phase in range(2):
        op_file = open(sys.argv[1], 'r')
        frame = 0
        for line in op_file:
            words = line.split()
            alloc = words[0].lower() == 'alloc'
            begin = int(words[1], 16)
            if alloc:
                end = int(words[2], 16)

                if phase == 1:
                    blocks[begin] = end

                if phase == 0:
                    if lower is None or begin < lower:
                        lower = begin
                    if upper is None or end > upper:
                        upper = end
            else:
                if phase == 1:
                    del blocks[begin]

            if phase == 1:
                filename = 'frame.%05d.png'%frame
                print 'Rendering "%s"...'%filename
                render_frame(blocks, filename, (lower, upper))
                frame = frame + 1
