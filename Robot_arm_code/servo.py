from pio_servo import PIO_SERVO
import time

import machine

class Servo:
    def __init__(self,sm_id, pin, angle):
        self.id = sm_id
        self.pin = pin
        self.servo = PIO_SERVO(self.id, self.pin)
        self.servo.set_angle(angle)
        self.default_angle = angle
        print("Servo {} initialized".format(sm_id))
    
    def set_angle(self, angle):
        self.servo.set_angle(angle)
    
    def get_angle(self):
        return self.servo.get_angle()
    
    def test_servo(self):
        """
        A tester function for servo. It tests the servo by changing the angle in all possible ways.

        Args:
            self: The servo that is going to be tested.
        """
        while 1:
            for i in range(-90, 90):
                print("Angle is {}".format(i))
                self.set_angle(i)
                time.sleep(0.1)
            
            
    

def test_servo_1_and_2(servo1:Servo, servo2:Servo):
    """
    A tester function to test the base servo (Servo1) and the second servo(Servo2).

    Args:
        servo1 (Servo): The base servo with PIN 16
        servo2 (Servo): The second servo with PIN 17.
    """
    
    while 0:
        time.sleep(3)
    while 1:
        servo1.set_angle(0)
        time.sleep(1)
        servo2.set_angle(90)
        time.sleep(1)
        servo1.set_angle(45)
        time.sleep(1)
        servo2.set_angle(45)
        time.sleep(1)
        servo1.set_angle(90)
        time.sleep(1)
        servo2.set_angle(0)
        time.sleep(1)

            
def go_get_it():
    """
    A grab and leave function for our further work. It grabs a piece of paper from a 
    proper position. Then it takes the paper to a wanted position.
    """
    servo1 = Servo(0,16)
    servo2 = Servo(1, 17)
    servo3 = Servo(2, 18)
    servo4 = Servo(3, 19)
    servo5 = Servo(4,20)
    servo6 = Servo(5,21)
    
    
    servo1.set_angle(-90)
    time.sleep(1)
    servo2.set_angle(60)
    time.sleep(1)
    servo3.set_angle(90)
    time.sleep(1)
    servo4.set_angle(50)
    time.sleep(1)
    servo5.set_angle(0)
    time.sleep(1)
    servo6.set_angle(-90)
    time.sleep(2)
    servo6.set_angle(0)
    time.sleep(1)
    servo2.set_angle(0)
    time.sleep(1)
    servo1.set_angle(30)
    time.sleep(1)
    servo6.set_angle(-90)