#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Generate project files for movie_from_ref
"""

from __future__ import absolute_import, print_function, unicode_literals

import os
import sys

from makeprojects import makeprojects

# List of Windows projects to generate
LIST_WINDOWS = [
    ("win32", "vs2022", "Release_LTCG"),
    ("win32", "watcom", "Release"),
    ("win32", "codewarrior", "Release")
]

# List of MacOSX projects to generate
LIST_MACOSX = [
    ("macosx", "xc3", "Release")
]

########################################


def do_generate(working_directory, arg_lists):
    """
    Generate all the project files in a folder

    Args:
        working_directory: Directory to place the project files.
        arg_lists: List of arguments for makeprojects
    """

    for item in arg_lists:
        args = ["-p", item[0], "-g", item[1], "-c", item[2]]
        makeprojects(working_directory=working_directory, args=args)

########################################


def generate(working_directory):
    """
    Generate all the project files.

    Args:
        working_directory: Directory to place the project files.
    """

    print("Generating projects for movie_from_ref")
    do_generate(
        os.path.join(working_directory, "movie_from_ref"),
        LIST_WINDOWS + LIST_MACOSX)

    print("Generating projects for QTSDK-master")
    do_generate(
        os.path.join(working_directory, "QTSDK-master"),
        LIST_WINDOWS + LIST_MACOSX)

    print("Generating projects for QTTimeCode")
    do_generate(
        os.path.join(working_directory, "QTTimeCode"),
        LIST_WINDOWS)
    return 0


########################################


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(generate(os.path.dirname(os.path.abspath(__file__))))
