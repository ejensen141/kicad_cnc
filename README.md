# KICAD CNC
A library to convert pad and placement data from kicad to gcode for a solder paste dispenser and pick and place machine.

Here is "plain" code:
This takes the data from a kicad board and outputs the pad positions.
It then writes Gcode to send to the CNC device.
![alt tag](images/nonoptimized.png)
As you can see however, the code it produces is random, and the machine must take large jumps between parts; somthing a human stuffing the board by hand would not do! Below you will find an example of the optimize algorthim in this library. Once the code is optimized, the machine will compute the shortest path to process the entire board. 
![alt tag](images/optimized.png)
