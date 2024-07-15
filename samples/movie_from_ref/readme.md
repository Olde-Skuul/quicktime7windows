# CreateMovieFromReferences

This sample is originally sourced from the Apple Developer Connection July 2009 CD.

- [Source 1 from Github](https://github.com/fruitsamples/CreateMovieFromReferences)
- [Source 2 from Github](https://github.com/cellularmitosis/ADC-reference-library-2009-july/tree/main/samplecode/CreateMovieFromReferences/CreateMovieFromReferences)

Updated by Rebecca Ann Heineman [becky@burgerbecky.com](mailto:becky@burgerbecky.com)

## OVERVIEW

Developers often need to be able to create a QuickTime movie from a series of video frames, where the frames are stored in a video track in the movie. Typically, this is accomplished by first creating a new movie file, and then calling ``AddMediaSample`` multiple times to add each of the video frames (as sample data) to the media in the video track. However, a faster technique is available.

This sample shows how to create a QuickTime movie from a series of video frames  where the media for the video track is the frame data in the file itself (in QuickTime terminology we say the media for the video track has the file as its "data reference" or data source).

## HOW IT WORKS

Here's how to create a movie using this technique:

- Create a movie and a video track
- Create the media for the video track (with ``NewTrackMedia``), pass an alias handle containing an alias to the video frames file for the dataRef and rAliasType for the dataRefType.
- Call ``AddMediaSampleReferences`` (or ``AddMediaSampleReferences64`` if your file can be bigger than 2GB) with a suitable image description and sample numbers, offsets, etc. to add your video frames as sample data to the track media
- Write the movie atom to the file and save

## SPECIAL CONSIDERATIONS

As just described, you create a Movie with a video track in memory, with media for the video track that has the file as its data reference. However, the first 16 bytes of the video frames file must be overwritten with the bit pattern that QuickTime will later recognize as the start of the 'mdat' atom (the atom that specifies the movie sample data), which it will patch when the movie resource is appended to the file:

(Note: these are big-endian)

```
    0x00000008
    0x77696465
    0x00000000
    0x6d646174
```

Then the first compressed video sample can be written at byte offset 16 in the file (the above atom data occupies the first 16 bytes). ``AddMediaSampleReferences64`` should be called to add sample references to the video media once approximately 1 MB of video samples have been accumulated.

When all of the compressed samples have been written to the file, the eof marker should be set to the end of the last compressed video sample. Then ``AddMovieToStorage`` can be called to cause the 'mdat' atom to be properly updated and the movie resource to be appended to the file.

## ABOUT THE "VideoFrames.bin" FILE

The file "VideoFrames.bin" included with this sample in the folder "data" contains video frames with the following characteristics:

| Label             | Value  |
| ----------------- | ------ |
| width             | 200    |
| height            | 240    |
| MediaTimeScale    | 600    |
| DurationPerSample | 60     |
| NumberOfSamples   | 60     |
| CodecType         | 'raw ' |

This file also contains the special 16-byte 'mdat' atom at the beginning of the file. When replacing the VideoFrames file with your own frames file you must of course update the above media values in the sample code to properly describe your particular media, and recompile. Also, make sure and include the 'mdat' atom at the start of your file.

## BUILDING THE SAMPLE

The sample is built as a command-line tool on both Mac and Windows.

Projects are generated using ``makeprojects``. Run the python script ``generate_projects.py`` to regenerate them.

On Macintosh, an Xcode 3.1.4 project is provided. 

On Windows, Microsoft Visual Studio 2022 C++, Open Watcom 1.9, and Codewarrior 9 for Windows projects are provided.

## RUNNING THE SAMPLE

This command-line tool expects only an "-input" parameter which specifies the video frames file from which the movie will be constructed. A sample video frames file "VideoFrames" is provided. 

**IMPORTANT:** When you run the tool the specified video frames file is converted into a QuickTime movie -- you need only append the ".mov" extension to the file name after running the tool and it should open in QuickTime Player when you double-click the file. The file is **OVERWRITTEN**

On Macintosh, you can run the tool from the Terminal application. Here's how to run the tool from the Terminal specifying the included video frames file:

1) Set the directory to the folder containing the sample code project "movie_from_ref"
2) Launch the tool and specify the "VideoFrames.bin" frames file as the input file like this:

``$ ./bin/movie_from_refxc3osx -input VideoFrames.bin``

On Windows, you can run the tool from the Command Prompt like this:

1) Set the directory to the folder containing the sample code project "movie_from_ref"
2) Launch the tool and specify the "VideoFrames" frames file as the input file like this:

``C:\samples\movie_from_ref> bin\movie_from_refv22ltc -input VideoFrames.bin``

## OTHER NOTES

When compressing using B-frames where media has different decode and display times you can't use ``AddMediaSampleReferences``. Instead, you'll need to use the newer QT Sample Table APIs. The QTSampleTable object now replaces the arrays of references for ``AddMediaSampleReferences``.

For more information, see the QuickTime 7 Update Guide.
