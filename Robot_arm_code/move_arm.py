from arm import Arm
import time
import machine
import sys


def move_arm(inp:str, arm:Arm):
    """
    Move the arm in the direction of given input.

    Args:
        inp (str): The string command. It may be u for up or d for down. Otherwise the command is
        stay still.
        arm (Arm): The arm (or let us call the servo) that is going to make movement.
    """
    init_angle = arm.get_angle()
    if (inp == "pos"): 
        arm.set_angle(init_angle - 20)
    elif (inp =="neg"):
        arm.set_angle(init_angle +20)
    else:
        arm.set_angle(init_angle)
    
    print(arm.get_angle())



def parse_input (input_serial:str, arms: list[Arm]):

    # TODO: ADD A CONSTRAINT CHECK ABOUT INPUT SIZE
    """
    The function that parses input coming from Leap Motion and moves the arms.

    Args:
        input (str): The input string consists of 4 words. The first two words describe
        the arm number and movement created by the left hand. The second two words do
        the same for the right hand. For each tuple, the first element represents the arm 
        that will be used and the second element represents the movement.

        arms (list[Arm]): The list of arms to control.
    """

    parsed = input_serial.rstrip().split(" ")
    left_servo = parsed[0]
    left_movement = parsed[1]
    
    right_servo = parsed[2]
    right_movement = parsed[3]
    
    # Movement parsing for Left Hand
    if(left_servo == "GRABBER"):
        if(left_movement == "GRAB"):
            move_arm("pos", arms[5])
        elif(left_movement == "RELEASE"):
            move_arm("neg", arms[5])
        else:
            move_arm("none", arms[5])
    else:
        None

    # Movement parsing for Right Hand
    
    if(right_servo == "BASE"):
        if(right_movement == "SWIPE_LEFT"):
            move_arm("pos", arms[0])
        elif(right_movement == "SWIPE_RIGHT"):
            move_arm("neg", arms[0])
        else:
            move_arm("still", arm[0])
    elif(right_servo == "LOWER_ARM"):
        if(right_movement == "UP"):
            move_arm("pos", arms[1])
        elif(right_movement == "DOWN"):
            move_arm("neg", arms[1])
        else:
            move_arm("still", arms[1])

    elif(right_servo == "MIDDLE_ARM"):
        if(right_movement == "UP"):
            move_arm("pos", arms[2])
        elif(right_movement == "DOWN"):
            move_arm("neg", arms[2])
        else:
            move_arm("still", arms[2])

    elif(right_servo == "HIGHER_ARM"):
        if(right_movement == "UP"):
            move_arm("pos", arms[3])
        elif(right_movement == "DOWN"):
            move_arm("neg", arms[3])
        else:
            move_arm("still", arms[3])
    elif(right_arm == "ROTOR"):
        if(right_movement == "SWIPE_LEFT"):
            move_arm("pos", arms[4])
        elif(right_movement == "SWIPE_RIGHT"):
            move_arm("neg", arms[4])
        else:
            move_arm("still", arms[4])
    else:
        right_arm = None
    

def main():
    

    # Initialize arms in a nice position
    arm1 = Arm(0, 16, 0)
    arm2 = Arm(1, 17, 90)
    arm3 = Arm(2, 18, -30)
    arm4 = Arm(3, 19, 0)
    arm5 = Arm(4, 20, -30)
    arm6 = Arm(5, 21, 0)
    
    # Add arms to the list
    arm_list = [arm1, arm2, arm3, arm4, arm5, arm6]

    arm6.test_arm()
    while(True):
        # Get the input
        input_serial = input("Enter the movement ")
    
        # Parse the input and make the movement.
        parse_input(input_serial, arm_list)
    
    
main()