import sys
from servo import Servo
from mover import move_servo
import time


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
     #print(gesture)
    if(gesture == "GRAB"):
        left_servo.set_angle(0)
    elif(gesture == "RELEASE"):
        left_servo.set_angle(-90)
    
    else:
        print("k")
        pass


def parse_right(servo, gesture, servos):
    print(servo)
    print(gesture)
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
    elif(servo == "ROTOR"):
    
        if(gesture == "SWIPE_LEFT"):
            move_servo("pos", servos[4])
        elif(gesture == "SWIPE_RIGHT"):
            move_servo("neg", servos[4])
        else:
            move_servo("still", servos[4])
            
    else:
        pass
    
    

def parse_tuple(tuple, servos):
    if(tuple[0] == "GRABBER"):
        parse_left(tuple[1], servos[5])
    else:
        parse_right(tuple[0], tuple[1] , servos)
        
def test_parser(servo_list):
    msg1 = "GRABBER RELEASE"
    msg2 = "GRABBER GRAB BASE SWIPE_LEFT"
    msg3 = "GRABBER RELEASE LOWER_ARM UP" 
    msg4 = "GRABBER GRAB HIGHER_ARM DOWN"
    msg5 = "GRABBER RELEASE ROTOR SWIPE_LEFT"
    msg6 = "BASE SWIPE_LEFT"
    msg7 = "BASE SWIPE_RIGHT GRABBER GRAB"
    msg8 = "ROTOR SWIPE_LEFT"
    msg9 = "MIDDLE_ARM UP"
    msg10 = "GRABBER GRAB"
    
    msg_list = [msg1, msg2, msg3, msg4, msg5, msg6, msg6, msg7, msg8,msg9, msg3, msg4]
    msg_list2 = [msg1, msg10]
    while(True):
        for msg in msg_list2:
            print(msg)
            parse_input(msg, servo_list)
            time.sleep(0.15)
        
def main():
    
    # Initialize servos in a nice position
    servo1 = Servo(0, 16, 0)
    servo2 = Servo(1, 17, 90)
    servo3 = Servo(2, 18, -30)
    servo4 = Servo(3, 19, 0)
    servo5 = Servo(4, 20, -30)
    servo6 = Servo(5, 21, 0)
    time.sleep(1)
    servo_list = [servo1, servo2, servo3, servo4, servo5, servo6]
    
    
    while(True):
        
        test_parser(servo_list)
        
        '''
        input_list = []
        input_msg = sys.stdin.readline().strip()
        if(input_msg != None):
            print(input_msg)
            input_list.add(input_msg)
            
            fst_input = input_li
            parse_input(input_msg,servo_list)
            #time.sleep(0.5)
        '''

main()
