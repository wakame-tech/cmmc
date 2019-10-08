#!/usr/bin/env python

import subprocess
import os.path
# $ pip install pychalk
from chalk import *

# `dirname $0`
pwd = os.path.realpath(os.path.dirname(__file__))
# pl0 VM's path
VM_PATH = pwd + '/../vm/pl0vm'
# pl0 test files' path
EX_PATH = pwd +'/../examples'

# test cases
# <filename> <stdins> <stdouts>
cases = [
    ('q1', [1, 2], [3]),
    ('q2', [1, 2, 3], [6]),
    ('q3', [1, 2, 3], [7]),
    ('q4', [15, 5], [5]),
    ('q5', [18, 6], [18]),
    ('q6', [10], [55]),
    ('q7', [1, 3, 8, 4, 2, 8, 5, 7, 9, 0], [9]),
    ('q8', [4, 6, 2, 3, 1], [1, 2, 3, 4, 6]),
    ('q9', [3], [23]),
    ('q10', [1, 1], [4]),
    ('q11', [5], [5]),
    ('q12', [3], [27]),
    ('q13', [2], [3]),
]

# inputs case to echo string
def escape(inputs):
    return '\"' + "\\n".join(map(str, inputs)) + '\\n\"'

# stdout bytes to \n splitted int array
def unescape(stdout):
    l = stdout.decode("utf-8").strip().split("\n")
    try:
        return list(map(int, l))
    except:
        return []

passed_count = 0
for (name, inputs, expects) in cases:
    file = name + '.pl0'
    path = EX_PATH + '/' + file
    if not os.path.exists(path):
        print('{} {} not found'.format(red("[Error]"), file))
        continue

    cmd = "echo {} | {} {}".format(escape(inputs), VM_PATH, path)
    stdout, stderr = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).communicate()

    if stderr:
        print('{} {}'.format(red("[Error]"), red(file)))
        print(stderr)
        continue

    # Asseting
    actual = unescape(stdout)

    if expects == actual:
        result = green("[Passed]")
        passed_count += 1
    else:
        result = red("[Failed]")
    
    print('{} {} (input: {}, expects: {}, actual: {})'.format(
        result,
        file, 
        str(inputs),
        str(expects),
        str(actual),
    ))

print('\n🍺  {} / {} Test passed!'.format(
    white(passed_count), 
    white(str(len(cases)))
))