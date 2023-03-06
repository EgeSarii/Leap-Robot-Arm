import sys
from servo import Servo
from mover import Mover
import time

class Parser:



    #Initialize mover
    mover = Mover()

    def parse_input (self,input_serial:str, servos: list[Servo]) -> None:

        parsed = input_serial.rstrip().split(" ")
        theta1 = float(parsed[0])
        theta2 = float(parsed[1])
        theta3 = float(parsed[2])
        theta4 = float(parsed[3])
        movement1 = parsed[4]
        movement2 = parsed[5]

        servos[0].set_angle(theta1)
        servos[1].set_angle(theta2)
        servos[2].set_angle(theta3)
        servos[3].set_angle(theta4)
        self.mover.move_servo(movement1, servos[4])
        if(movement2 == "open"):
            servos[5].set_angle(180)
        else:
            servos[5].set_angle(0)

    




        