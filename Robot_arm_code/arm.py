from pio_servo import PIO_SERVO
import time

import machine

class Arm:
    def __init__(self,sm_id, pin, angle):
        self.id = sm_id
        self.pin = pin
        self.servo = PIO_SERVO(self.id, self.pin)
        self.servo.set_angle(angle)
        self.default_angle = angle
        print("Arm {} initialized".format(sm_id))
    
    def set_angle(self, angle):
        self.servo.set_angle(angle)
    
    def get_angle(self):
        return self.servo.get_angle()
    

def test_arm_1_and_2(arm1:Arm, arm2:Arm):
    """
    A tester function to test the base servo (Arm1) and the second servo(Arm2).

    Args:
        arm1 (Arm): The base servo with PIN 16
        arm2 (Arm): The second servo with PIN 17.
    """
    
    while 0:
        time.sleep(3)
    while 1:
        arm1.set_angle(0)
        time.sleep(1)
        arm2.set_angle(90)
        time.sleep(1)
        arm1.set_angle(45)
        time.sleep(1)
        arm2.set_angle(45)
        time.sleep(1)
        arm1.set_angle(90)
        time.sleep(1)
        arm2.set_angle(0)
        time.sleep(1)


def test_arm(arm:Arm):
    """
    A tester function for arm. It tests the servo by changing the angle in all possible ways.

    Args:
        arm (Arm): The arm/servo that is going to be tested.
    """
    while 1:
        for i in range(-90, 90):
            print("Angle is {}".format(i))
            arm.set_angle(i)
            time.sleep(0.1)
            
            
def go_get_it():
    """
    A grab and leave function for our further work. It grabs a piece of paper from a 
    proper position. Then it takes the paper to a wanted position.
    """
    arm1 = Arm(0,16)
    arm2 = Arm(1, 17)
    arm3 = Arm(2, 18)
    arm4 = Arm(3, 19)
    arm5 = Arm(4,20)
    arm6 = Arm(5,21)
    
    
    arm1.set_angle(-90)
    time.sleep(1)
    arm2.set_angle(60)
    time.sleep(1)
    arm3.set_angle(90)
    time.sleep(1)
    arm4.set_angle(50)
    time.sleep(1)
    arm5.set_angle(0)
    time.sleep(1)
    arm6.set_angle(-90)
    time.sleep(2)
    arm6.set_angle(0)
    time.sleep(1)
    arm2.set_angle(0)
    time.sleep(1)
    arm1.set_angle(30)
    time.sleep(1)
    arm6.set_angle(-90)
    