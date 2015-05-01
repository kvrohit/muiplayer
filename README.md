# MUI #

**MUI**, _MUI mUsic Interface_, is an open-source cross platform media player written using Qt and FMOD.

### Current status: ###
version 0.0.3 (source)<br />
Public versions: 0.0.2 (binaries)

### Compiling ###
**Linux**: Download and copy the FMOD .so files to /usr/lib and execute the 'makeall' script in the root MUI directory.

**Windows**: Download and install FMOD. Run the 'makeall.bat' file.

### Known Issues: ###
0.0.3:
  * Some features broken due to code overhaul (fixed)
  * Does not accept/render files with unicode filenames
  * For some files, meta data is not correctly extracted

0.0.2:
  * UI Rendering bugs in Ubuntu 8.04/Qt 4.3.4
  * Unicode encoded tags not rendered properly (under testing)

0.0.1:
  * Does not handle M3U v1 playlists gracefully (fixed)
  * Drag and Drop buggy in Win32 (fixed)
  * Crashes while saving playlist, when the Duration column is empty (fixed)

### Changelog: ###
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
