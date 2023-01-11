from servo import Servo
import time
import machine
import sys


class Mover:

    def __init__(self, coefficient=10) -> None:
        self.coefficient = coefficient

    def move_servo(self,inp: str, servo: Servo)-> None:
        """
        Move the servo in the direction of given input.

        Args:
            inp (str): The string command. It may be u for up or d for down. Otherwise the command is
            stay still.
            servo (Servo): The servo that is going to make movement.
        """
        init_angle = servo.get_angle()
        if (inp == "pos"):
            if (init_angle >= -85):
                servo.set_angle(init_angle - self.coefficient)  # TODO : OPTIMIZE ANGLE : 5 -> 2.5
        elif (inp == "neg"):
            if (init_angle <= 85):
                servo.set_angle(init_angle + self.coefficient)  # TODO : OPTIMIZE ANGLE : 5 -> 2.5
        else:
            pass

        print(servo.get_angle())


    def move_robot_to_initial(self,servo_list: list[Servo])-> None:
        """
        Move robot to its initial positions by setting the angles of the servos to their 
        first initialised values.

        Args:
            servo_list (list[Servo]): The servo list that contains the servos of the robot arm.
        """
        
        self.move_slowly(servo_list[2], -30)
        self.move_slowly(servo_list[1], 30)
        self.move_slowly(servo_list[0],60)
        
        time.sleep(0.5)
        for servo in servo_list:
            self.move_slowly(servo, servo.initial_angle)


    def set_angle_coefficient(self,coefficient: int) -> None:
        self.coefficient = coefficient
    
    def get_angle_coefficient(self)-> int:
        return self.coefficient
    
    def move_slowly(self, servo:Servo, aim:int) -> None:
        """
        Move the servo to a specific angle, aim but in slow movements.

        Args:   
            servo (Servo): The servo to be moved
            aim (int): The angle to be aimed
        """
        init_angle = servo.get_angle()
        if (init_angle < aim):
            while(servo.get_angle() <= aim):
                servo.set_angle(servo.get_angle() + 10)
                time.sleep(0.05)
        else:
            while(servo.get_angle() >= aim):
                servo.set_angle(servo.get_angle() - 10)
                time.sleep(0.05)
    
