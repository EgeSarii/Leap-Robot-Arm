#### Week 4(26 September - 2 October) 
- Research and small implementation on Leap Motion is done.
- 6 DOF Makerfabs robot is ordered and started to assemble it.

#### Week 5(3 October - 9 October)
- 6 DOF Makerfabs robot is fully assembled
- Raspberry Pi Pico is installed and set up some example code is run and tested.
- Implementation on Leap Motion is started. The main goal is to find and implement hand gestures in leap motion corresponding to the commands for the robot arm.

#### Week 6(10 October - 16 October)
- Leap motion implementation is continued, a progress is achieved by defining some positions and gestures.
- PIO for Robot servos are created. Also some basic movements implemented for base servos.

#### Week 7(17 October - 23 October)
- Detecting movement and classifying it for the right hand  is implemented successfully in Leap Motion. The main challenge was to find which gesture was detected in Leap Motion.
- A grap and get movement implemented in Robot arm by using all the servos simultaneously. The arm goes for a specific location, get closes to the target, graps it and then take it to the destination and leaves there smoothly.
- A program with a basic user interface to control all the arms is implemented. Now by using this program (move_arm.py), you can get a tile from the floor move to wherever you want. This is the core of our project goal.
- All the movements are discussed and decided, the implementation for all the combination is started.


#### Week 8(7 November - 13 November)
- The architecture of Leap Motion is adjusted for communication between Raspberry Pi Pico. 
- Research is done to find a possible way of communication between Raspberry Pi Pico and the Leap Motion. General architecture for this communication is created.

#### Week 9 (14 November- 20 November)
- The communication between Leap Motion is established. It is tested and it is a working implementation. The implementation consists of two parts. The first part is to create a correct format of the message to send and create a connector to the Pico. The second part is to create a parser for the incoming messages from Leap Motion in Pico.
- The code for Leap Motion is adjusted to be cleaner after the connectors established.

#### Week 10 (21 November - 27 November)
- Work on communication between Leap Motion and Robot is improved. Latancy issues are checked and solve.
The issue consisted in the Pico receiving inputs too quickly and hence the microprocessor was not able to set new positions before the new command was received.
This was solved by:
  1. Eliminating dummy commands {STILL, NONE} and sending less input.
  2. Making the machine which processes the Leap motion frames sleep for a brief time and hence discard some new frames.

Option 2) was implemented for the left_hand. In this hand there is no circular buffer (because no movements require a sequence of actions) and hence plenty of frames of the same command were detected.
 By sleeping, most of  this duplicates were removed.

An alternative is to implement another buffer to store commands and
fil this buffer up before writing to serial.

The writing operation occurs once this buffer is filled.
During the writings, the listener attached to the Leap motion can be
removed and hence the Leap Motion Device can be "paused" from
detecting new frames.
After every write operation, a small time is left to the subsequent
process and execution of the command. This time can be provided by
calling the function *sleep()*.
