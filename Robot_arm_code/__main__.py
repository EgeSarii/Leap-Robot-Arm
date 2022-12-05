from servo import Servo
from parser import Parser
import time
import sys

def main():
    
    

    #Initialize parser
    parser = Parser()

    # Initialize servos in a nice position
    servo1 = Servo(0, 16, 0)
    servo2 = Servo(1, 17, 90)
    servo3 = Servo(2, 18, -30)
    servo4 = Servo(3, 19, 0)
    servo5 = Servo(4, 20, -30)
    servo6 = Servo(5, 21, -90)
    time.sleep(1)
    servo_list = [servo1, servo2, servo3, servo4, servo5, servo6]
    
    
    while(True):
        
        #test_parser(servo_list)
        
        input_msg = sys.stdin.readline().strip()
        if(input_msg != ""):
            print(input_msg)
            parser.parse_input(input_msg,servo_list)
            #time.sleep(0.5)
            


if __name__ == "__main__":
    main()