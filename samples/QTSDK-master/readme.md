# QTSDK

This sample was taken from PixelOmen's repository [https://github.com/PixelOmen/QTSDK](https://github.com/PixelOmen/QTSDK). Updated to use the modern Quicktime SDK using Visual Studio 2022. **NOTE** This sample is written in modern C++ and won't compile with Open Watcom or Codewarrior

## What is QTSDK
QTSDK is a command-line program that utilizes the QuickTime 7 SDK to perform various operations on QuickTime files on Windows.

## Features

QTSDK supports a variety of operations on QuickTime files, including but not limited to:

- Audio channel flagging
- Setting start timecode
- Optional arguments for 6CH M&E and Disney 24Ch layout

## Requirements

- CMake
- MSVC compiler
- Windows 7 or higher
- QuickTime 7 installed and properly configured

## Usage

QTSDK requires specific command-line arguments to operate on QuickTime files. The general syntax for commands is as follows, with additional arguments required for certain operations:

- `-path "c:\path\to\qt.mov"`: Specifies the path to the QuickTime file.
<br><br>
- `-flagaudio -channels X,Y`: Flags the specified audio channels in the QuickTime file. `X,Y` represents the channel range to be flagged, seperated by a comma `,`
<br><br>
- `-setTC -fps X -tc "HH:MM:SS:FF"`: Sets the timecode of the QuickTime file with the specified frames per second (fps) and timecode (tc).
<br><br>

##### Optional arguments:

- `-me6ch`: Specifies that there are 6 channels of M&E and they should be flagged as L/R/L/R/L/R. Requires channel range to be 6 channels, e.g. `1,6`
<br><br>
- `-disney24`: Specifies that there are 24 channels and they should be flagged per Disney layout specs

### Example Commands

```bash
QTSDK -path "C:\path\to\file.mov" -flagaudio -channels 1,2
QTSDK -path "C:\path\to\file.mov" -setTC -fps 24 -tc "00:00:10:00"
QTSDK -path "C:\path\to\file.mov" -flagaudio -me6ch -channels 1,6
QTSDK -path "C:\path\to\file.mov" -flagaudio -disney24 -channels 1,24
```