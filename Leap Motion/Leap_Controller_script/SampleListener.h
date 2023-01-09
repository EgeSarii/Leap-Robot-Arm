#ifndef SAMPLELISTENER_H
#define SAMPLELISTENER_H

#include <stdlib.h>
#include <iostream>
#include "Leap.h"

//Defining Listener Sub-class: 
class SampleListener : public Leap::Listener {
    public:
      virtual void onConnect(const Leap::Controller&);
      virtual void demo(const Leap::Frame&);
      virtual void onFrame(const Leap::Controller&);
};
#endif

/*
  ############################################
  ## EXPLANATION OF EXISTENCE OF SUCH CLASS ##
  ############################################

The text below follows from the online documentation of the Leap
Motion Sensor. 

   The connection process is asynchronous and hence a method to obtain
   the data from the sensor is required. In this script a "Listener"
   is used.
 
   A Listener can be added to the Controller, which provides an
   event-based mechanism for responding to important Controller state
   changes.

   Listeners are not always the best idea.
   The reason are:

	1. Listeners use idependent threads to invoke the implemented
           code for each method. When using Listeners, a thread-safe
           programming style should be used.

        2. Overhead might occur when using threads.


  As alternative to Listeners, it is possible to get the Controller
  object for frames.

In order to use Listeners, the Listener class must be present.
This class presents the signatures of the functions which should be 
overwritten.

The advantage of using a listener is that the Leap Motion controller
will only send and process data if any change is detected.
This means that without a Leap::Listener the Leap Motion will be
constantly processing. In this way, the Leap motion controller will
operates only on new positions.

This was the chosen method because it saves a little of energy and computations.
*/

