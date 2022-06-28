--------------------------------------------------------------------------------
Task:    Pop-Art Image Generator (Medium Task)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This program generates a pop-art ppm-image from a given ppm-image. 
This program is only suitiable for P6-ppm images (Binary).
The RGB-components are rearranged according to the RGB-string provided via 
command line arguments.

--------------------------------------------------------------------------------

How to compile:
gcc -Wall -Wextra -pedantic -o popgen main.c checkprogargs.c ppmp6io.c rgbhandling.c -I.

--------------------------------------------------------------------------------

Usage:
  linux terminal: 
    ./popgen <inputfilename>.ppm <outputfilename>.ppm <RGB-string>
  windows cmd:
    popgen.exe <inputfilename>.ppm <outputfilename>.ppm  <RGB-string>

--------------------------------------------------------------------------------

Example (linux terminal):
./popgen image.ppm test.ppm grb:brg:rbg:grb

Pop-art image successfully generated!

--------------------------------------------------------------------------------

