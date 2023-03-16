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
Vector EMPTY[BUFSIZE] = { Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(),
                          //Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector()
                        };

/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

Vector position_buffer[BUFSIZE] = { Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(),
                                    //Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector()
                                  };

unsigned int buffer_index  = 0;

std::mutex serial_mutex;

std::mutex buffer_mutex;

//This will obtain the process id once the function "open" is called.
int serial = 0;
