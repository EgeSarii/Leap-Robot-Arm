#include <unistd.h>
#include <cstdlib>
#include <mutex>
#include <string.h>
#include "Leap.h"
#include "primitives.h"

using namespace Leap;
/*
    ####################
    # GLOBAL VARIABLES #
    ####################
*/

Vector position_buffer[BUFSIZE] = { Vector(), Vector(), Vector(), Vector(), Vector(), Vector(), Vector()};

unsigned int buffer_index  = 0;

std::mutex serial_mutex;

std::mutex buffer_mutex;

std::mutex coordinates_mutex;

//This will obtain the process id once the function "open" is called.
int serial = 0;
