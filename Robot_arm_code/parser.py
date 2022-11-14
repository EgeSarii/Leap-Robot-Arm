import sys
from servo import Servo
from mover import move_servo


def parse_input (input_serial:str, servos: list[Servo]):
    
    #TODO : ADD A CONSTRAINT CHECK ABOUT INPUT SIZE
    """
    The function that parses input coming from Leap Motion and moves the arm.

    Args:
        input (str): The input string consists of 4 words. The first two words describe
        the arm number and movement created by the left hand. The second two words do
        the same for the right hand. For each tuple, the first element represents the arm 
        that will be used and the second element represents the movement.

        arms (list[Servo]): The list of servos to control.
    """

    parsed = input_serial.rstrip().split(" ")

    if(len(parsed) ==2):
        parse_tuple(parsed, servos)

    elif (len(parsed) == 4):
        movement1 = (parsed[0], parsed[1])
        movement2 = (parsed[2], parsed[3])
        parse_tuple(movement1,servos)
        parse_tuple(movement2, servos)

    else:
        print("Input format is not correct!")    
    
    # Movement parsing for Right Hand
    


def parse_left(gesture, left_servo):
    if(gesture == "GRAB"):
        move_servo("pos", left_servo)
    elif(gesture == "RELEASE"):
        move_servo("neg", left_servo)
    else:
        move_servo("none", left_servo)


def parse_right(servo, gesture, servos):

    if ( servo== "BASE"):
        if (gesture == "SWIPE_LEFT"):
            move_servo("pos", servos[0])
        elif (gesture == "SWIPE_RIGHT"):
            move_servo("neg", servos[0])
        else:
            move_servo("still", servos[0])
    elif(servo == "LOWER_ARM"):
        if(gesture == "UP"):
            move_servo("pos", servos[1])
        elif(gesture == "DOWN"):
            move_servo("neg", servos[1])
        else:
            move_servo("still", servos[1])

    elif(servo == "MIDDLE_ARM"):
        if(gesture == "UP"):
            move_servo("pos", servos[2])
        elif(gesture == "DOWN"):
            move_servo("neg", servos[2])
        else:
            move_servo("still", servos[2])

    elif(servo == "HIGHER_ARM"):
        if(gesture == "UP"):
            move_servo("pos", servos[3])
        elif(gesture == "DOWN"):
            move_servo("neg", servos[3])
        else:
            move_servo("still", servos[3])
    elif(right_servo == "ROTOR"):
        if(gesture == "SWIPE_LEFT"):
            move_servo("pos", servos[4])
        elif(gesture == "SWIPE_RIGHT"):
            move_servo("neg", servos[4])
        else:
            move_servo("still", servos[4])
    else:
        right_servo = None

def parse_tuple(tuple, servos):
    if(tuple[0] == "GRABBER"):
        parse_left(tuple[1], servos[5])
    else:
        parse_right(tuple[0], tuple[1] , servos)
        
        
# Initialize servos in a nice position
servo1 = Servo(0, 16, 0)
servo2 = Servo(1, 17, 90)
servo3 = Servo(2, 18, -30)
servo4 = Servo(3, 19, 0)
servo5 = Servo(4, 20, -30)
servo6 = Servo(5, 21, 0)

servo_list = [servo1, servo2, servo3, servo4, servo5, servo6]


parse_input("LOWER_ARM UP GRABBER STILL",servo_list)
