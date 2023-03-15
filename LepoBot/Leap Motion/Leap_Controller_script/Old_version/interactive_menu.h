#ifndef INTERACTIVE_MENU
#define INTERACTIVE_MENU

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>

#define PATH "./Files/"


//Function which reads a content of a file "filename" and stores it in
//'content'. True is returned iff the file is read correctly.
bool read_file(const std::string filename, std::string & content);


//Function to display possible commands from the main menu of the application.
void display_menu_actions();


//Function to display possible commands from the help page of the application.
void display_help_actions();


//Function to display possible commands from the demo page of the application.
void display_demo_actions();


//Function to display main page of application.
void menu_page();


//Function to display help page of application.
void help_page();


//Function used to ask new input in case of wrong commands.
//The iterative application uses recursion and because of this there
//is no loop in here.
std::string handle_wrong_input(std::string action);


//Function which handles the options when the application is in the
//"menu_flow" state.
bool menu_state(std::string action, std::string & state, const std::string ROBOT_TEXT);


//Function which handles the options when the application is in the
//"tutorial_flow" state.
bool tutorial_state (std::string action, std::string &state,  const std::string & GESTURE_TEXT, const std::string & ROBOT_LYRICS_TEXT, const std::string & ROBOT_TEXT );


//Function which handles the options when the application is in the
//"demo_flow" state.
bool demo_state(std::string action, std::string & state, const std::string ROBOT_TEXT);


//Function in which the logic of the FSM of the application is
//handled.
bool execute_action (std::string  action, std::string & state, const std::string & GESTURE_TEXT, const std::string & ROBOT_LYRICS_TEXT, const std::string & ROBOT_TEXT );

//Function in which the menu is started and continues until an "exit"
//condition is met.
void interactive_menu();

#endif
