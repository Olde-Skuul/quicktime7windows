#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Build rules for the makeprojects suite of build tools.

This file is parsed by the cleanme, buildme, rebuildme and makeprojects
command line tools to clean, build and generate project files.

When any of these tools are invoked, this file is loaded and parsed to
determine special rules on how to handle building the code and / or data.
"""

# pylint: disable=unused-argument

from __future__ import absolute_import, print_function, unicode_literals

import sys
import os

from burger import delete_file, clean_directories
from makeprojects.enums import ProjectTypes, IDETypes, PlatformTypes

# Type of the project, default is ProjectTypes.tool
PROJECT_TYPE = ProjectTypes.library

# Recommended IDE for the project. Default is IDETypes.default()
PROJECT_IDE = IDETypes.watcom

# Recommend target platform for the project.
PROJECT_PLATFORM = PlatformTypes.win32

# If set to True, Don't parse directories in this folder when ``-r``
# is active.
# Can be overridden above
NO_RECURSE = True

# If any IDE file is present, cleanme and buildme will process them.
# Can be overridden above
PROCESS_PROJECT_FILES = False

# Only build one project, for OpenWatcom
# List of projects to generate if makeprojects is invoked
# without any parameters
MAKEPROJECTS = (
    {"platform": "win32",
     "ide": "watcom",
     "type": "library"},
)

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

    # Delete the deployed file
    lib_dir = os.path.dirname(working_directory)
    lib_dir = os.path.join(lib_dir, "Libraries")
    delete_file(os.path.join(lib_dir, "QTWatcom.lib"))

    clean_directories(working_directory,
                      ("temp", "bin", ".vs", ".vscode"))
    return 0

########################################


def configuration_list(platform, ide):
    """
    Return names of configurations.

    The default is to generate configuration names of "Release",
    "Debug", etc. If an override is desired, return a list of strings
    containing the names of the default configurations.

    Args:
        platform: PlatformTypes of the platform being built
        ide: IDE project being generated for.
    Returns:
        None or list of configuration names.
    """

    # Only generate for the Release configuration
    return ["Release"]


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

    # The library needs access to the Quicktime headers
    configuration.include_folders_list.append("..\\CIncludes")

    # Deploy it to the library folder
    configuration.post_build = [
        "Copying QTWatcom.lib...",
        "copy /y $(TargetPath) ..\\Libraries\\QTWatcom.lib"
    ]
    return 0


# If called as a command line, replace 0 with a call the function
# for the default action. Return a numeric error code, or zero.
if __name__ == "__main__":
    sys.exit(0)
