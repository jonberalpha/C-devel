--------------------------------------------------------------------------------
Task:    Distributed Sensing (System Programming)
Author:  Berger Jonas
--------------------------------------------------------------------------------

Description:
This Task consists of multiple Sensor-Tasks and one Processing-Task.
The Sensor-Tasks write the Sensor-Values in a message queue
The Processing-Task reads the values from the queue and
if there are conversion-flags set (given as program arguments) then
they are converted into the corresponding unit.
In addition an svg-image which shows the mean-value of the humidity-, pressure- 
and temperature-values, an svg-image which shows the last 10 values of each 
measurement (needs to be at least 10 values of any measurement-type to create 
the file) and a txt-file which logs all the received data, are created. 
Moreover every received value is displayed on the console followed 
by the calculated current mean-values.

--------------------------------------------------------------------------------

Building:
Build the executable-programs using:
make

Delete created executables using:
make clean

--------------------------------------------------------------------------------

Usage:
Sensor-Task:
The sensor tasks program supports the following command-line options:
-H ... measure humidity data
-P ... measure pressure data
-T ... measure temperature data
-d ... add a conversion-duration in seconds between each measurement

At least one of -H , -P , and -T is required for launching the program

Examples:
./sens -H -P -T
./sens -P
./sens -T -d 3

Processing-Task:
The processing task program supports the following command-line options:
-c ... convert temperature to Celsius before generating output
       (not to be used together with -f)
-f ... convert temperature to Farenheit before generating output
       (not to be used together with -c)
-a ... convert pressure to ATMs before generating output

Examples:
./proc -c -a
./proc -f

--------------------------------------------------------------------------------

Example-Executions:

Sensor-program (processing program has to be started first):
./sens -H -P -T
Sensor Task:
Sensor-Value 45 with Measure-Type 1 sent!
Sensor-Value 114840 with Measure-Type 2 sent!
Sensor-Value 254 with Measure-Type 3 sent!
(EXITS)

Processing-program:
./proc
Processing Task:
Raw Humidity-Value received: 45 %
Mean-Values: H: 45.0 % P: 0.000 PA T: 0.0 K
Raw Pressure-Value received: 114840 PA
Mean-Values: H: 45.0 % P: 114840.000 PA T: 0.0 K
Raw Temperature-Value received: 254 K
Mean-Values: H: 45.0 % P: 114840.000 PA T: 254.0 K
(continues)

