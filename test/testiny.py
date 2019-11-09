"""
  testiny.py - tiny testing script
"""
__author__ = "wakame_tech <4617023@ed.tus.ac.jp>"
__status__ = "development"
__version__ = "0.0.1"
__date__    = "09 November 2019"

from abc import abstractmethod
import subprocess
import os.path
from glob import glob
# $ pip install pychalk
from chalk import *
import re

class Testiny:
    # test cases
    # <filename> <stdins> <stdouts>
    def __init__(self, target, cases, env = {}):
        self.target = target
        self.cases = cases
        self.env = env

    # inputs case to echo string
    def escape_inputs(self, inputs):
        return '\"' + '\\n'.join(map(str, inputs)) + '\\n\"'

    # stdout bytes to \n splitted int array
    def unescape_outouts(self, output):
        l = output.split("\n")
        try: return list(map(int, l))
        except: return []

    def exec(self, cmd, stdin = ''):
        if stdin: cmd = 'echo %s | %s' % (stdin, cmd)

        # replace env
        matches = re.finditer(r'\$([a-zA-Z_]+)', cmd)
        for m in matches:
            v = m.group(1)
            if v in self.env:
                cmd = cmd.replace('$%s' % v, self.env[v])
            else:
                raise Exception()

        stdout, stderr = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).communicate()
        if stderr: raise stderr
        return stdout.decode('utf-8').strip()

    @abstractmethod
    def pipeline(path, inputs):
        raise NotImplementedError()

    def test(self):
        passed_count = 0
        for path in glob(self.target):
            filename = os.path.basename(path)
            if not filename in self.cases:
                continue

            print('%s %s' % (yellow('[TEST]'), filename), end=' ')

            inputs, outputs = self.cases[filename]
            try:
                result = self.pipeline(path, inputs)
            except Exception as e:
                print('%s', red('[ERROR]'))
                print(e)
                continue

            if outputs == result:
                print('%s' % green('✔ Passed'))
                passed_count += 1
            else:
                print('%s' % red('✗ Failed'))

            print('inputs: %s' % inputs)
            print('expect: %s' % outputs)
            print('actual: %s' % result)
            print('')
        
        print('\n🍺  %s / %s Test passed!' % (passed_count, str(len(self.cases))))