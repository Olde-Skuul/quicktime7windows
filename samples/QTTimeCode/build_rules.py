#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Configuration file on how to build and clean projects in a specific folder.

This file is parsed by the cleanme, buildme, rebuildme and makeprojects
command line tools to clean, build and generate project files.
"""

# pylint: disable=global-statement
# pylint: disable=unused-argument

from __future__ import absolute_import, print_function, unicode_literals

import os

from makeprojects import ProjectTypes
from burger import create_folder_if_needed, get_windows_host_type, \
    run_command

# Type of the project, default is ProjectTypes.app
PROJECT_TYPE = ProjectTypes.app

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
     "ide": ("vs2003", "vs2022"),
     "configuration": "Release_LTCG"},
    {"platform": "win32",
     "ide": ("watcom", "codewarrior"),
     "configuration": "Release"
     }
)

########################################


def postbuild(working_directory, configuration):
    """
    Issue build commands after all IDE projects have been built.

    This function can assume all other build projects have executed for final
    deployment or cleanup

    On exit, return 0 for no error, or a non zero error code if there was an
    error to report. Return None to allow a parent folder's postbuild() function
    to execute.

    Args:
        working_directory
            Directory this script resides in.

        configuration
            Configuration to build, ``all`` if no configuration was requested.

    Returns:
        None if allowing parent folder to execute, otherwise an integer error
        code.
    """

    # Codewarrior for Windows doesn't support post build commands,
    # so do it here.
    if get_windows_host_type():

        # Create the path
        outputdir = os.path.join(working_directory, "bin")

        # Make the folder
        create_folder_if_needed(outputdir)

        # Only run if the file exists
        test_file = os.path.join(outputdir, "QTTimeCodec50w32rel.exe")
        if os.path.isfile(test_file):

            # Invoke the rez compiler
            cmd = [
                "..\\..\\Tools\\rez",
                "-o", "bin\\QTTimeCodec50w32rel.exe",
                "source\\QTTimeCode.r"
            ]

            error, _, _ = run_command(cmd, working_directory)
            return error

    # Do nothing on non Windows platforms
    return None


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

    # Add the source for the Windows version
    project.source_folders_list.append("source/windows")

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

    # Run the Rez file script after building the executable
    configuration.post_build = [
        "Building source\\QTTimeCode.r",
        "..\\..\\Tools\\rez -o \"$(TargetPath)\" source\\QTTimeCode.r"
    ]
