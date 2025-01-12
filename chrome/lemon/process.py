#!/usr/bin/env python
# Copyright (c) 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Run objcopy --add-gnu-debuglink for the NaCl IRT.
"""

import subprocess
import sys


def Main(args):
  crypto_exe, input_file, output_dir,file1,file2 = args
  print crypto_exe
  print input_file
  print output_dir
  print file1
  print file2
  return subprocess.call([
      crypto_exe, input_file, output_dir,file1,file2
      ])


if __name__ == '__main__':
  sys.exit(Main(sys.argv[1:]))
