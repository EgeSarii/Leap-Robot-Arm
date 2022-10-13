from pio_servo import PIO_SERVO
import time

import machine

class Arm:
    def __init__(self,sm_id, pin):
        self.id = sm_id
        self.pin = pin
        self.servo = PIO_SERVO(self.id, self.pin)
        print("Arm {} initialized".format(sm_id))
    
    def set_angle(self, angle):
        self.servo.set_angle(angle)
    

def test_arm_1_and_2(arm1, arm2):
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

arm1 = Arm(0,16)
arm2 = Arm(1,17)
test_arm_1_and_2(arm1, arm2)
