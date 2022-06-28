--------------------------------------------------------------------------------
Task:    Password Generator (Simple Task)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This program generates a password from given template string and password length 
provided via command line.

--------------------------------------------------------------------------------

How to compile:
gcc -Wall -pedantic -Wextra -o pwgen main.c

--------------------------------------------------------------------------------

Usage:
  linux terminal: 
    ./pwgen <template> <length>
  windows cmd:
    pwgen.exe <template> <length>

--------------------------------------------------------------------------------

Example Executions (linux terminal):
./pwgen a 8
ahuuxxtc

./pwgen.exe aA 40
GmheIQEPPIdghNAAkgYwrYkuQUTVuzGpnbWjRVzf

./pwgen.exe 7.aX 40
F&ufbRq+=W!cQ&PSob%S3Ic)7J>J@d9ei/fg{s-S

--------------------------------------------------------------------------------

