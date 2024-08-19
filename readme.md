# Quicktime for Windows 7.3.0.70

Linkable SDK for Quicktime 7 for Windows. Created so projects using github can link to this repository. This has been modernized to build with Visual Studio 2003-2022, Codewarrior for Windows, and Open Watcom 1.9.

Send bug reports, comments, and insults to [Rebecca Ann Heineman becky@burgerbecky.com](mailto:becky@burgerbecky.com)

## Compatibility

Quicktime for Windows only runs on 32 bit Intel versions of Windows. It does not exist for 64 bit Intel or ARM.

This is based on the last known public release of the QuickTime for Windows SDK. Modifications were made to ensure compatiblity to modern SDKs and compilers.

This has been tested with Visual Studio 2003-2022, Open Watcom 1.9, Codewarrior 9 for Windows, Clang, and GCC

## How to build

### Prerequisites on MacOSX and Windows

* Install Python 3
* ``pip install -U makeprojects``

### To regenate the projects

Run ``makeprojects`` in the folders WatcomLibraries, movie_from_ref, QTSDK-master, or QTTimeCode. All the projects will be regenerated if files were added or removed.

### Windows

Visual Studio 2022 or OpenWatcom 1.9 or Metrowerks CodeWarrior 9.4 for Windows

Open the sample projects, build, and run.

### MacOS

For movie_from_ref and QTSDK-master, XCode for MacOS X. For QTTimeCode, Metrowerks CodeWarrior 10.0 (Can be run from Mac OS X Snow Leopard).

Open the samples projects, build, and run. QTTimeCode runs as a MacOS Carbon App, so MacOS X up to Snow Leopard can run it.



