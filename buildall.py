#!/usr/bin/env python
# encoding: utf-8

import subprocess

def build_with_cmake():
    targets = ['img_rcv', 'img_sender']
    builddir = 'build'

    cmake_setup = ['cmake', '-GNinja', '-B' + builddir]
    cmake_compile = ['ninja', '-C', builddir]
    subprocess.check_call(cmake_setup)
    subprocess.check_call(cmake_compile + targets)


if __name__ == '__main__':
    build_with_cmake()
