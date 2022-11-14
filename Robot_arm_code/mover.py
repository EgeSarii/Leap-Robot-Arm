from servo import Servo
import time
import machine
import sys



def move_servo(inp:str, servo:Servo):
    """
    Move the servo in the direction of given input.

    Args:
        inp (str): The string command. It may be u for up or d for down. Otherwise the command is
        stay still.
        servo (Servo): The servo that is going to make movement.
    """
    init_angle = servo.get_angle()
    if (inp == "pos"): 
        servo.set_angle(init_angle - 20)
    elif (inp =="neg"):
        servo.set_angle(init_angle +20)
    else:
        servo.set_angle(init_angle)
    
    print(servo.get_angle())



def parse_input (input_serial:str, servos: list[Servo]):

    # TODO: ADD A CONSTRAINT CHECK ABOUT INPUT SIZE
    """
    The function that parses input coming from Leap Motion and moves the arm.

    Args:
        input (str): The input string consists of 4 words. The first two words describe
        the servo number and movement created by the left hand. The second two words do
        the same for the right hand. For each tuple, the first element represents the servo 
        that will be used and the second element represents the movement.

        servos (list[Servo]): The list of servos to control.
    """

    parsed = input_serial.rstrip().split(" ")
    left_servo = parsed[0]
    left_movement = parsed[1]
    
    right_servo = parsed[2]
    right_movement = parsed[3]
    
    # Movement parsing for Left Hand
    if(left_servo == "GRABBER"):
        if(left_movement == "GRAB"):
            move_servo("pos", servos[5])
        elif(left_movement == "RELEASE"):
            move_servo("neg", servos[5])
        else:
            move_servo("none", servos[5])
    else:
        None

    # Movement parsing for Right Hand
    
    if(right_servo == "BASE"):
        if(right_movement == "SWIPE_LEFT"):
            move_servo("pos", servos[0])
        elif(right_movement == "SWIPE_RIGHT"):
            move_servo("neg", servos[0])
        else:
            move_servo("still", servo[0])
    elif(right_servo == "LOWER_ARM"):
        if(right_movement == "UP"):
            move_servo("pos", servos[1])
        elif(right_movement == "DOWN"):
            move_servo("neg", servos[1])
        else:
            move_servo("still", servos[1])

    elif(right_servo == "MIDDLE_ARM"):
        if(right_movement == "UP"):
            move_servo("pos", servos[2])
        elif(right_movement == "DOWN"):
            move_servo("neg", servos[2])
        else:
            move_servo("still", servos[2])

    elif(right_servo == "HIGHER_ARM"):
        if(right_movement == "UP"):
            move_servo("pos", servos[3])
        elif(right_movement == "DOWN"):
            move_servo("neg", servos[3])
        else:
            move_servo("still", servos[3])
    elif(right_servo == "ROTOR"):
        if(right_movement == "SWIPE_LEFT"):
            move_servo("pos", servos[4])
        elif(right_movement == "SWIPE_RIGHT"):
            move_servo("neg", servos[4])
        else:
            move_servo("still", servos[4])
    else:
        right_servo = None
    

def main():
    

    # Initialize servos in a nice position
    servo1 = Servo(0, 16, 0)
    servo2 = Servo(1, 17, 90)
    servo3 = Servo(2, 18, -30)
    servo4 = Servo(3, 19, 0)
    servo5 = Servo(4, 20, -30)
    servo6 = Servo(5, 21, 0)
    
    # Add servos to the list
    servo_list = [servo1, servo2, servo3, servo4, servo5, servo6]

    servo6.test_servo()
    while(True):
        # Get the input
        input_serial = input("Enter the movement ")
    
        # Parse the input and make the movement.
        parse_input(input_serial, servo_list)
    
    
main()