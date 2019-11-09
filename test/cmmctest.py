#!/usr/bin/env python

import os.path
from testiny import Testiny

class Test(Testiny):
    def pipeline(self, path, inputs):
        self.exec('$cmmc %s' % path)
        stdout = self.exec('$vm %s' % (pwd + '/a.out'), self.escape_inputs(inputs))
        return self.unescape_outouts(stdout)

cases = {
    'array.cmm': [[1, 2, 3, 4], [1, 3, 2, 4]],
    'for.cmm': [[], [3, 2, 1]],
}

# `dirname $0`
pwd = os.path.realpath(os.path.dirname(__file__))

env = {
    # cmmc path
    'cmmc': pwd + '/../compiler/cmmc',
    # pl0vm path
    'vm': pwd +'/../vm/pl0vm',
}

# test file path
TARGET = pwd + '/../examples/cmm/*'

Test(TARGET, cases, env).test()