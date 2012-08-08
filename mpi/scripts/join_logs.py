#!/usr/bin/env python

import os

if __name__ == '__main__':

    inputs = [e for e in os.listdir('.') if e[:8] == 'mpi_log.']

    entries = []
    for filename in inputs:
        with open(filename) as input_file:
            for line in input_file:
                if line.strip():
                    words = line.split()
                    entries.append((float(words[0]), line))

    entries.sort(key=lambda x: x[0])

    with open('mpi_log', 'w') as output_file:
        for entry in entries:
            output_file.write(entry[1])
