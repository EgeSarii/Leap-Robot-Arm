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
        servo.set_angle(init_angle -20)
    elif (inp =="neg"):
        servo.set_angle(init_angle +20)
    else:
        pass
    
    print(servo.get_angle())
    
def move_robot_to_initial(servo_list: list[Servo]):
    
    servo_list[0].set_angle(0)
    servo_list[1].set_angle(90)
    servo_list[2].set_angle(-30)
    servo_list[3].set_angle(0)
    servo_list[4].set_angle(-30)
    servo_list[5].set_angle(0)
