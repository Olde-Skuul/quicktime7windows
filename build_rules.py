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
# pylint: disable=consider-using-f-string

from __future__ import absolute_import, print_function, unicode_literals

import sys
import os

from makeprojects import PlatformTypes, IDETypes
from burger import environment_root

# If any IDE file is present, cleanme and buildme will process them.
# Can be overridden above
PROCESS_PROJECT_FILES = False

# Note: Make sure the Watcom shim is built before anything else

# Process these folders before processing this folder
# Can be overridden above
DEPENDENCIES = ["WatcomLibraries"]

########################################


def library_settings(configuration):
    """
    Add settings when using this project at a library

    When configuration.library_rules_list[] is set to a list of
    directories, if the directory has a build_rules.py file, it
    will run this function on every configuration to add the
    library this rules file describes.

    Args:
        configuration: Configuration class instance to update.

    Returns:
        None, to continue processing, zero is no error and stop processing,
        any other number is an error code.
    """

    # pylint: disable=too-many-branches

    # Where is this file located?
    this_dir = os.path.dirname(os.path.abspath(__file__))

    # If located in BURGER_SDKS, use that, otherwise
    # the relative pathname
    root_path, env_var = environment_root(
        this_dir, "BURGER_SDKS", configuration.working_directory)

    # Folders for the libraries and includes for Quicktime for Windows
    lib_dir = root_path + "\\Libraries"
    include_dir = root_path + "\\CIncludes"

    # Special folder only for Watcom
    watcom_dir = root_path + "\\WatcomLibraries"

    # If an environment variable was used, ensure the IDE tests
    # for it, just in case.
    if env_var and "BURGER_SDKS" not in configuration.env_variable_list:
        configuration.env_variable_list.append("BURGER_SDKS")

    # Get the IDE and platform
    ide = configuration.project.solution.ide
    platform = configuration.platform

    # Only modify Windows Intel 32 bit platform since Quicktime
    # isn't supported on ARM or 64 bit platforms
    if platform is PlatformTypes.win32:

        # Watcom uses a standard library that differs from the one
        # the original library was compiled with. To build, use
        # this shim that's built against the Watcom stdlib
        if ide is IDETypes.watcom:
            configuration.libraries_list.append("WatcomLibrarieswatw32rel.lib")
            configuration.library_folders_list.append(watcom_dir)

        # Add in the real library file and directory for Windows
        configuration.libraries_list.append("QTMLClient.lib")
        configuration.library_folders_list.append(lib_dir)

        # Include headers, however Codewarrior uses the library folder
        # to support the used of #include <QTML.h> syntax
        if configuration.project.solution.ide.is_codewarrior():
            configuration.library_folders_list.append(include_dir)
        else:
            configuration.include_folders_list.append(include_dir)
        return None

    # All other Windows platforms are not supported by Quicktime
    if platform.is_windows():

        # These IDEs only support 32 bit Intel so they ignore
        # ARM and Intel 64
        if ide.is_codewarrior() or ide is IDETypes.watcom:
            return None

        print("# Error: Quicktime for Windows is 32 bit Intel only")
        return 15

    # MacOS supports Quicktime, so always pass
    # and include the Quicktime framework
    if platform.is_macos() or platform.is_darwin():

        if ide.is_xcode():

            # Quicktime is only supported on 32 bit platforms
            if platform.is_macosx():
                # Only Xcode 3 supports powerpc
                if ide < IDETypes.xcode4:
                    configuration.project.xc_archs = ["ppc", "i386"]
                # Force 32 bit intel only
                else:
                    configuration.project.xc_archs = ["i386"]

            # Make sure QuickTime is in the list of frameworks
            if "QuickTime.framework" not in configuration.frameworks_list:
                configuration.frameworks_list.append("QuickTime.framework")
        return None

    # Otherwise, Quicktime is not available
    # pylint: disable=line-too-long
    print(
        "# Error: Quicktime is only available on Windows 32 bit or Darwin platforms")
    return None

########################################


def command_line():
    """
    Called when directly executed.
    """

    # pylint: disable=line-too-long

    # Print usage
    print(
        "This file contains the rules to add Quicktime for Windows to your makeprojects\n"
        "project. On Darwin/MacOS platforms, this library module will do nothing. On\n"
        "Windows Intel x86 it will add the Quicktime for Windows library settings\n\n"
        "On all other Windows platforms, it will generate an error since Quicktime is\n"
        "not supported on those platforms."
    )
    return 0

########################################


# If called as a command line and not a class, perform the build
if __name__ == "__main__":
    sys.exit(command_line())
