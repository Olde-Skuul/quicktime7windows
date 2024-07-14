#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Configuration file on how to build and clean projects in a specific folder.

This file is parsed by the cleanme, buildme, rebuildme and makeprojects
command line tools to clean, build and generate project files.
"""

# pylint: disable=global-statement

from __future__ import absolute_import, print_function, unicode_literals

import os

from burger import clean_directories, is_under_git_control, clean_files, \
    clean_codeblocks, clean_xcode
from makeprojects import ProjectTypes

# Type of the project, default is ProjectTypes.tool
PROJECT_TYPE = ProjectTypes.tool

# ``cleanme`` and ``buildme`` will process build_rules.py in the parent folder
# if True. Default is false
# Can be overridden above
CONTINUE = False

# Both ``cleanme`` and ``buildme`` will process any child directory with the
# clean(), prebuild(), build(), and postbuild() functions if True.
# Can be overridden above
GENERIC = True

# ``cleanme`` will assume only the function ``clean()`` is used if False.
# Overrides PROCESS_PROJECT_FILES
CLEANME_PROCESS_PROJECT_FILES = False

# Check if git is around
_GIT_FOUND = None

########################################


def is_git(working_directory):
    """
    Detect if perforce or git is source control

    Returns:
        True if found, False if not.
    """

    # Cached result
    global _GIT_FOUND

    if _GIT_FOUND is None:
        _GIT_FOUND = is_under_git_control(working_directory)
    return _GIT_FOUND

########################################


def clean(working_directory):
    """
    Delete temporary files.

    This function is called by ``cleanme`` to remove temporary files.

    On exit, return 0 for no error, or a non zero error code if there was an
    error to report.

    Args:
        working_directory
            Directory this script resides in.

    Returns:
        None if not implemented, otherwise an integer error code.
    """

    clean_directories(
        working_directory,
        (".vscode", "temp", "ipch", "bin", ".vs", "*_Data",
         "* Data", "__pycache__"))

    clean_files(
        working_directory,
        (".DS_Store", "*.suo", "*.user", "*.ncb", "*.err",
         "*.sdf", "*.layout.cbTemp", "*.VC.db", "*.pyc", "*.pyo"))

    # If Doxygen was found using this directory, clean up
    if os.path.isfile(os.path.join(working_directory, "Doxyfile")):
        clean_files(
            working_directory,
            ("doxygenerrors.txt", "*.chm", "*.chw", "*.tmp"),
            recursive=True)

    # Check if the directory has a codeblocks project file and clean
    # codeblocks extra files
    clean_codeblocks(working_directory)

    # Allow purging user data in XCode project directories
    clean_xcode(working_directory)

########################################


def project_settings(project):
    """
    Set up defines and default libraries.

    Adjust the default settings for the project to generate. Usually it's
    setting the location of source code or perforce support.

    Args:
        project: Project record to update.

    Returns:
        None, to continue processing, zero is no error and stop processing,
        any other number is an error code.
    """

    # Makeprojects defaults to perforce on
    project.solution.perforce = not is_git(project.working_directory)

    # Disable Visual Studio warnings
    if project.platform.is_windows():
        project.define_list.extend((
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS"))

########################################


def configuration_settings(configuration):
    """
    Set up defines and libraries on a configuration basis.

    For each configation, set all configuration specific seting. Use
    configuration.name to determine which configuration is being processed.

    Args:
        configuration: Configuration class instance to update.

    Returns:
        None, to continue processing, zero is no error and stop processing,
        any other number is an error code.
    """

    # Where is this file?
    this_dir = os.path.dirname(os.path.abspath(__file__))

    # Add the root folder to link in the library relative to this folder
    configuration.library_rules_list.append(
        os.path.dirname(this_dir))
