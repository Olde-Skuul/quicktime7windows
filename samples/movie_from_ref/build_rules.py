#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Configuration file on how to build and clean projects in a specific folder.

This file is parsed by the cleanme, buildme, rebuildme and makeprojects
command line tools to clean, build and generate project files.
"""

# pylint: disable=global-statement

from __future__ import absolute_import, print_function, unicode_literals

# ``cleanme`` and ``buildme`` will process build_rules.py in the parent folder
# if True. Default is false
# Can be overridden above
CONTINUE = True

# List of projects to generate if makeprojects is invoked
# without any parameters, default create recommended
# project for the host machine

# Create windows projects for Watcom, VS 2022, and Codewarrior
MAKEPROJECTS = (
    {"platform": "win32",
     "ide": "vs2022",
     "configuration": "Release_LTCG"},
    {"platform": "win32",
     "ide": ("watcom", "codewarrior"),
     "configuration": "Release"},
    {"platform": "macosx",
     "ide": "xc3",
     "configuration": "Release"}
)
