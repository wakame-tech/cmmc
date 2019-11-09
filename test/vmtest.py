#!/usr/bin/env python

import os.path
from testiny import Testiny

# `dirname $0`
pwd = os.path.realpath(os.path.dirname(__file__))

# pl0 test files' path
TARGET = pwd +'/../examples/pl0/*'

env = {
    # pl0 VM's path
    'vm': pwd + '/../vm/pl0vm'
}

cases = {
    'q1.pl0': [[1, 2], [3]],
    'q2.pl0': [[1, 2, 3], [6]],
    'q3.pl0': [[1, 2, 3], [7]],
    'q4.pl0': [[15, 5], [5]],
    'q5.pl0': [[18, 6], [18]],
    'q6.pl0': [[10], [55]],
    'q7.pl0': [[1, 3, 8, 4, 2, 8, 5, 7, 9, 0], [9]],
    'q8.pl0': [[4, 6, 2, 3, 1], [1, 2, 3, 4, 6]],
    'q9.pl0': [[3], [19]],
    'q10.pl0': [[1, 1], [4]],
    'q11.pl0': [[5], [8]],
    'q12.pl0': [[3], [27]],
    'q13.pl0': [[2], [3]],
}

class Test(Testiny):
    def pipeline(self, path, inputs):
        stdout = self.exec('$vm %s' % path, self.escape_inputs(inputs))
        return self.unescape_outouts(stdout)

Test(TARGET, cases, env).test()
