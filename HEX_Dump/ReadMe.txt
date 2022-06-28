--------------------------------------------------------------------------------
Task:    Hex Dump (Simple Task)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This program prints out a hex dump from a given file provided via command line.

--------------------------------------------------------------------------------

How to compile:
gcc -Wall -pedantic -Wextra -o hexdump main.c

--------------------------------------------------------------------------------

Usage:
  linux terminal: 
    ./hexdump <filename>.<type>
  windows cmd:
    hexdump.exe <filename>.<type>

--------------------------------------------------------------------------------

Example (linux terminal):
./hexdump file.txt
00000000   23 69 6e 63 6c 75 64 65   #include
00000008   20 3c 73 74 64 69 6f 2e   .<stdio.
00000010   68 3e 0a 23 69 6e 63 6c   h>.#incl
00000018   75 64 65 20 3c 73 74 64   ude.<std
00000020   69 6e 74 2e 68 3e 0a 23   int.h>.#
...
continues till end of given file

--------------------------------------------------------------------------------

