#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string.h>

#include "Leap.h"
#include "SampleListener.h"
#include "primitives.h"
#include "inverse_kinematics.h"

using namespace Leap;

void SampleListener::onConnect(const Controller &controller)
{
    std::cout << "CONNECTED\n" << std::endl;
}

void SampleListener::onFrame(const Controller& controller)
{
  const Frame frame = controller.frame();
  update_hands_position(frame);  
}
