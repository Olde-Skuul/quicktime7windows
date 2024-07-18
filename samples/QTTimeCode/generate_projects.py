#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Generate project files for QTTimeCode
"""

from __future__ import absolute_import, print_function, unicode_literals

import os
import sys

from makeprojects import makeprojects

########################################


def generate(working_directory):
    """
    Generate all the project files.

    Args:
        working_directory: Directory to place the project files.
    """

    # The MacOS project is pre-made
    arg_lists = (
        ("win32", "vs2022", "Release_LTCG"),
        ("win32", "watcom", "Release"),
        ("win32", "codewarrior", "Release")
    )

    for item in arg_lists:
        args = ["-p", item[0], "-g", item[1], "-c", item[2]]
        makeprojects(working_directory=working_directory, args=args)
    return 0


########################################


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(generate(os.path.dirname(os.path.abspath(__file__))))
