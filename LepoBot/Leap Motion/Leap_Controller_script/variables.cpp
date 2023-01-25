#include <unistd.h>
#include <cstdlib>
#include <mutex>
#include <string.h>
#include "Leap.h"
#include "datatypes.h"

using namespace Leap;

//Location of serial device to be written.
std::string SERIAL_ID = "/dev/ttyACM0";

//EMPTY array of Leap::vectors to clean buffer.
Vector EMPTY[BUFSIZE] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

//This array of vectors will store the 10 previous vectors of a
//given frame. This is used in order to compute gestures and catch
//actions. Here Vector is a Leap::Vector; not std::vector.
//This is used as a circular buffer of size BUFSIZE.
Vector rightHandPositions[BUFSIZE] = {Vector(), Vector(), Vector(), Vector(), Vector(),Vector(), Vector(), Vector(), Vector(), Vector()};

//This is a circular buffer to print only 1 out of ten received
//commands. The Leap motion captures ~26 frames a second and without
//this buffer the Leap Motion will detect several movements related to
//the left hand. This is because the movements of the left hand are
//detected based on a single frame.
Vector left_hand_buffer[BUFSIZE];
  
//This variable is used to keep track of the indexes of such circular buffer.
unsigned rightHandPosIndex = 0;
unsigned left_hand_index  = 0;

//Mutex needed because inner class is actually executed in different tread.
std::mutex serial_mutex; //Needed to correctly print commands to
			 //serial and std::out. Without it we had
			 //issues.

std::mutex r_buffer_mutex; //Mutex for ensure synchronization on
			   //global variable 'rightHandPositions'.

//This will obtain the process id once the function "open" is called.
int serial = 0;

