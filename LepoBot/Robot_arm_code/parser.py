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

        
        while((servos[0].get_angle() != theta1) or
              (servos[1].get_angle() != theta2) or
              (servos[2].get_angle() != theta3) or
              (servos[3].get_angle() != theta4)):
        
        
            self.mover.move_slowly(servos[0], theta1)
            self.mover.move_slowly(servos[1], theta2)
            self.mover.move_slowly(servos[2], theta3)
            self.mover.move_slowly(servos[3], theta4)
            
        self.mover.move_servo(movement1, servos[4])
        if(movement2 == "open"):
            servos[5].set_angle(0)
        else:
            servos[5].set_angle(180)

    

        
