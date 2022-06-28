--------------------------------------------------------------------------------
Task:    Natural Language Clock (Medium Task)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This program converts the given time (24-hours-presentation) in 
a literal sentance.

--------------------------------------------------------------------------------

How to compile:
gcc -Wall -Wextra -pedantic -o clock main.c convargs.c genlit.c -I.

--------------------------------------------------------------------------------

Usage:
  linux terminal: 
    ./clock <hours> <minutes>
  windows cmd:
    clock.exe <hours> <minutes>

--------------------------------------------------------------------------------

Examples (linux terminal):
./clock 11 55
It is five minutes before noon.

./clock 0 0
It is midnight.

--------------------------------------------------------------------------------

