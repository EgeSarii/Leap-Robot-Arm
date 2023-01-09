#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string.h>

#include "Leap.h"
#include "SampleListener.h"
#include "primitives.h"
#include "interactive_menu.h"

using namespace Leap;

void SampleListener::onConnect(const Controller &controller)
{
    std::cout << "CONNECTED\n" << std::endl;
}

void SampleListener::demo(const Frame & frame)
{
  std::vector<std::string> practice_commands = { {"BASE SWIPE_LEFT "},
                                                 {"BASE SWIPE_RIGHT "},
    			   	                 {"LOWER_ARM UP "},
					         {"LOWER_ARM DOWN "},
					         {"MIDDLE_ARM UP "},
					         {"MIDDLE_ARM DOWN "},
					         {"HIGHER_ARM UP "},
					         {"HIGHER_ARM DOWN "},
 					         {"ROTOR SWIPE_LEFT "},
					         {"ROTOR SWIPE_RIGHT "},
					         {"GRABBER GRAB "},
					         {"GRABBER RELEASE "}
                                                };
  
  std::cout << "Waiting for connection...\n";
  
  for (int i = 0; i<10; i++)
  {
    const std::string demo_move = practice_commands[ rand() % 12 ];
    std::string message = "";
    std::cout << demo_move << "\n";
    while ( message != demo_move)
    {
      HandList hands = frame.hands();
      Movement demo_movement = assign_hands(hands[0]);
      message = show_movement(demo_movement);
      std::cout << message;
    }
    
    std::cout << "VERY GOOD!\n";
    sleep(2);
    
    if ( i < 9) {std::cout << "Next gesture is: ";}
  }

}

void SampleListener::onFrame(const Controller& controller)
{
  extern bool play_demo;
  const Frame frame = controller.frame();

  if (play_demo) { demo(frame);}
  else {update_hands_position(frame);}
  
}
