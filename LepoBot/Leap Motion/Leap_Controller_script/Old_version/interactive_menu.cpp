#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>

#include "Leap.h"
#include "SampleListener.h"
#include "datatypes.h"
#include "primitives.h"

/*
  This script consists in a terminal application to provide the user
  of LepoBot with basic notions and a small practice.
*/

using namespace Leap;

//Shared variable used to start the demo in the OnFrame() method of
//the SimpleListener.
bool play_demo = false;

//Function which reads a content of a file "filename" and stores it in
//'content'. True is returned iff the file is read correctly.
bool read_file(const std::string filename, std::string & content)
{
   std::ifstream infile;
   infile.open(PATH + filename);

   if (! infile.is_open() ) 
   {
     std::cout << "Error in opening filename \'" << filename << "\'.\n";
     return false;
   }

   while (! infile.fail())
   {
     std::string new_line = "";
     getline(infile, new_line, '\n');
     new_line += "\n";
     content += new_line;
   }

   infile.close();
   return true;
}

//Function to display possible commands from the main menu of the application.
void display_menu_actions()
{
  std::cout << "HELP := Displays movements and gestures that can be performed.\n"
            << "MENU := Come back to main page.\n"
            << "RUN  := Starts Leap Motion and gesture recognition to command\n"
            << "        the robotic arm.\n"
            << "EXIT := Exit program\n\n>";
}

//Function to display possible commands from the help page of the application.
void display_help_actions()
{
  std::cout << "GESTURES    := Page with list of gestures and small practice.\n"
            << "SCHEMATICS  := See robot schematics and components.\n"
            << "RUN         := Start demo section.\n"
            << "HELP        := Come back to help main page.\n"
	    << "MENU        := Come back to main menu.\n\n>";
}

//Function to display possible commands from the demo page of the application.
void display_demo_actions()
{
  std::cout << "START      := Begin the demo.\n"
            << "BACK       := Go back to \"HELP\" page.\n"
	    << "MENU       := Come back to main menu.\n\n>";
}

//Function to display main page of application.
void menu_page()
{
  std::cout << "WELCOME TO LEPOBOT!\n\n"
            << "LepoBot allows you to pilot a robotic arm 6-DOF by the use of a\n"
            << "Leap Motion Controller.\n\n"
            << "Here you can read information about which gestures are captured\n"
            << "by the Leap Motion Sensor and transferred to the robotic arm.\n\n"
            << "In the \"HELP\" page it is possible to find instructions and a\n"
            << "small tutorial to practice the detection of movements.\n\n"
            << "##########################################################\n"
            << "## You can always type \'h\' to obtain a list of possible ##\n"
            << "## commands to navigate in the program.                 ##\n"
            << "##########################################################\n\n"
            << "\tPLEASE ENTER ONE OF THE FOLLOWING ACTIONS:\n";

  display_menu_actions();
}

//Function to display help page of application.
void help_page()
{
  std::cout << "In this page you can find information about the gestures that\n"
            << "can be performed on the Leap Motion and schematics to move the\n"
            << "robotic arm.\n"
            << "Specific gestures can move specific parts of the robot.\n\n"
            << "Follow this brief tutorial to first have information and then\n"
            << "do a small practice with the Leap Motion.\n\n";

  display_help_actions();
}

//Function used to ask new input in case of wrong commands.
//The iterative application uses recursion and because of this there
//is no loop in here.
std::string handle_wrong_input(std::string action)
{
  std::cout << "Action is not recognize! Please try again.\n>";
  getline(std::cin, action);
  
  for (unsigned int c = 0; c < action.length(); c++)
    action[c] = toupper(action[c]);

  return action;
}

//Function which handles the options when the application is in the
//"menu_flow" state.
bool menu_state(std::string action, std::string & state, const std::string ROBOT_TEXT)
{  
  if (tolower(action[0]) == 'h' && action.length() == 1)
  {
     display_menu_actions();
     return true;
  }
    
  if (action == "HELP")
  {
     if (system("CLS")) system("clear");
     help_page();
     state = "tutorial_flow";
     return true;
   }
    
   if (action == "MENU")
   {
      if (system("CLS")) system("clear");
      menu_page();
      return true;
   }
    
   if (action == "RUN")
   {
      if (system("CLS"))system("clear");
      std::cout << ROBOT_TEXT;
      return false;
   }
    
   if (action == "EXIT")
   {
      if (system("CLS"))system("clear");
      return false;
   }

   action = handle_wrong_input(action);
   return menu_state(action, state, ROBOT_TEXT);
}

//Function which handles the options when the application is in the
//"tutorial_flow" state.
bool tutorial_state (std::string action, std::string &state,  const std::string & GESTURE_TEXT, const std::string & ROBOT_LYRICS_TEXT, const std::string & ROBOT_TEXT )
{
   if (tolower(action[0]) =='h' && action.length() == 1)
   {
      display_help_actions();
      return true;
   }
    
   if (action == "GESTURES")
   {
      if (system("CLS")) system("clear");
      std::cout << GESTURE_TEXT << ">";
      return true;
   }
    
   if (action == "SCHEMATICS")
   {
      if (system("CLS")) system("clear");
      std::cout << ROBOT_LYRICS_TEXT << ROBOT_TEXT << ">";
      return true;
   }
    
   if (action == "HELP")
   {
      state = "menu_flow";
      return menu_state("HELP", state, ROBOT_TEXT);
   }
    
   if (action == "MENU")
   {
      state = "menu_flow";
      return menu_state("MENU", state, ROBOT_TEXT);
   }

   action = handle_wrong_input(action);
   return tutorial_state(action, state, GESTURE_TEXT, ROBOT_LYRICS_TEXT, ROBOT_TEXT);
}

//Function which handles the options when the application is in the
//"demo_flow" state.
bool demo_state(std::string action, std::string & state, const std::string ROBOT_TEXT)
{
  
  if (tolower(action[0]) == 'h' && action.length() == 1){ display_demo_actions(); return true; }

  if (action == "DEMO_PAGE")
  {
     if (system("CLS")) {system("clear");}
     std::cout << "WELCOME TO THE DEMO!\n"
               << "Now a sequence of gestures will be presented to you.\n"
               << "Make the given gestures by capturing it with the Leap Motion.\n"
               << "There will be 10 randomly selected gestures.\n"
               << "Complete all of those to finish the practice! Once you have\n"
               << "done this, go back to the main menu (MENU command) and start using LEPOBOT!\n\n"
               << "Use the command START to start the practice!\n\n";
      
     display_demo_actions();
     return true;
  }
    
  if (action == "START")
  {
     std::cout << "The demo will start in 5 seconds. Get ready!\n";
     sleep(5);
     std::cout << "BEGIN!\n" << ROBOT_TEXT;

     SampleListener demo_listener;
     Controller controller;

     std::cout << "Waiting for connection...\n";
     
     controller.addListener(demo_listener);

     std::cin.get();

     controller.removeListener(demo_listener);
     play_demo = false;
     std::cout << "WELL DONE!\nGo back to the menu and start using LEPOBOT!\n\n>";
     return true;
  }
    
  if (action == "BACK")
  {
     play_demo = true;
     state = "menu_flow";
     return menu_state("HELP", state, ROBOT_TEXT);
  }

  if (action == "MENU")
  {
     play_demo = true;
     state = "menu_flow";
     return menu_state("MENU", state, ROBOT_TEXT);
   }
  
  action = handle_wrong_input(action);
  return demo_state(action, state, ROBOT_TEXT);
}

//Function in which the logic of the FSM of the application is
//handled.
bool execute_action (std::string  action, std::string & state, const std::string & GESTURE_TEXT, const std::string & ROBOT_LYRICS_TEXT, const std::string & ROBOT_TEXT )
{
  for (unsigned int c = 0; c < action.length(); c++) {action[c] = toupper(action[c]);}
  
  if (state == "menu_flow"){ return menu_state(action, state, ROBOT_TEXT);}
  
  if (state == "tutorial_flow")
  {
    if (action != "RUN") {return tutorial_state(action, state, GESTURE_TEXT, ROBOT_LYRICS_TEXT, ROBOT_TEXT);}
    
    state = "demo_flow";
    return execute_action("DEMO_PAGE", state, GESTURE_TEXT, ROBOT_LYRICS_TEXT, ROBOT_TEXT);;
  }
  
  if (state == "demo_flow") { return demo_state(action, state, ROBOT_TEXT);}

  action = handle_wrong_input(action);
  return execute_action(action, state, GESTURE_TEXT, ROBOT_LYRICS_TEXT, ROBOT_TEXT);
}

//Function in which the menu is started and continues until an "exit"
//condition is met.
void interactive_menu()
{
  std::string gestures = "", lyrics = "", robot = "";
  if ( read_file("Leap_arm_mappings.txt", gestures) && read_file("robot_explanation.txt", lyrics) && read_file("ascii-6-DOF.txt", robot))
  {
    menu_page();

    std::string action = "", state = "menu_flow";
    getline(std::cin,action);
    
    while(execute_action(action, state, gestures, lyrics, robot)) { getline(std::cin,action); }   
  }
}
