--------------------------------------------------------------------------------
Task:    Universal Image Filter (Hard Task)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This program generates a filtered image of a given original image.
The applied kernel filter is provided as command line arguments.
This program is only suitiable for P6-ppm images (Binary).

--------------------------------------------------------------------------------

How to compile:
gcc -Wall -Wextra -pedantic -o imfilter main.c checkprogargs.c ppmp6io.c imgfilter.c -lm -I.

--------------------------------------------------------------------------------

Usage:
  linux terminal: 
    ./imfilter <inputfilename> <outputfilename> <kernel-values>
  windows cmd:
    imfilter.exe <inputfilename> <outputfilename>  <kernel-values>

--------------------------------------------------------------------------------

Examples (linux terminal):

./imfilter image.ppm test.ppm -1,-1,-1,-1,8,-1,-1,-1,-1

Filtered image successfully generated!

./imfilter image.ppm test.ppm 0,0,0,0,1,0,0,0,0

Filtered image successfully generated!

./imfilter image.ppm test.ppm 0.111,0.111,0.111,0.111,0.111,0.111,0.111,0.111,0.111

Filtered image successfully generated!

--------------------------------------------------------------------------------

