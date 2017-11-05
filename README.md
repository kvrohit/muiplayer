[![Build Status](https://travis-ci.org/kvrohit/muiplayer.svg?branch=master)](https://travis-ci.org/kvrohit/muiplayer)

# MUI #

**MUI**, _MUI mUsic Interface_, is an open-source cross platform media player written using Qt

## Building ##
Clone the project and run the following commands, binaries are stored in `bin`

```sh
$ qmake
$ make
```

## Changelog ##
0.0.4:
  * Removed File system browser dock window
  * Changed media engine to `Qt Multimedia`
  * Removed 3rd party library dependencies, `FMOD` is out
  * Metadata handling is done by Qt
  * Support for several other file formats added (mp4, m4a, flac)
  * Source code structure cleanup

0.0.3:
  * Added File system browser dock window
  * Added a new toolbar to display the song meta data and album art
  * Added a new custom model class to handle song meta data more efficiently.
  * Fixed UI memory leaks.
  * Drag and drop is now handled by the model rather than the main window.
  * Minor issues fixed

0.0.2:
  * Added Unicode support for ID3v2.3
  * Added Metadata dockable window
  * Added ID3v2.3 support
  * Added dockable control toolbar
  * Added volume control
  * Added Drag and Drop folders
  * Tweaked interface

0.0.1:
  * Added M3U Support
  * Added Drag and Drop files
