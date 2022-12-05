import sys
from servo import Servo
from mover import Mover
import time

class Parser:



    #Initialize mover
    mover = Mover()

    def parse_input (self,input_serial:str, servos: list[Servo]) -> None:

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
            self.parse_tuple(parsed, servos)

        elif (len(parsed) == 4):
            movement1 = (parsed[0], parsed[1])
            movement2 = (parsed[2], parsed[3])
            self.parse_tuple(movement1,servos)
            self.parse_tuple(movement2, servos)

        else:
            print("Input format is not correct!")    
    


    def parse_tuple(self,tuple:list[str], servos: list[Servo])-> None:
        """ 
        Helper function to parse the tuple message. The first part of the tuple describes the servo
        and the second part describes the gesture.

        Args:
            tuple (list[str]): The tuple with size 2, contains servo and the gesture.
            servos (list[Servo]): The list that contains servos to be moved.
        """
        if(tuple[0] == "GRABBER"):
            self.parse_left(tuple[1], servos[5])
        elif(tuple[0] == "GO" and tuple[1] =="HOME"):
            self.mover.move_robot_to_initial(servos)
        elif(tuple[0] == "NONE" and tuple[1] == "CLAP"):
            message = ("The coefficient for the movement angle is now {} .\n").format(self.mover.get_angle_coefficient())
            message= message + "Enter the new coefficient!"
            new_coefficient = input(message)
            new_coefficient = int(new_coefficient)
            self.mover.set_angle_coefficient(new_coefficient)
        else:
            self.parse_right(tuple[0], tuple[1] , servos)

    def parse_left(self,gesture:str, left_servo:Servo)-> None:
        """Helper function to parse the message created for left hand commands.

        Args:
            gesture (str): The gesture that defines the movement : GRAB or RELEASE.
            left_servo (Servo): The servo (which is the grabber servo) to be moved
        """
        #print(gesture)
        if(gesture == "GRAB"):
            left_servo.set_angle(0)
        elif(gesture == "RELEASE"):
            left_servo.set_angle(-90)
        
        else:
            pass


    def parse_right(self,servo:Servo, gesture:str, servos: list[Servo])-> None:
        """ 
        Helper function to parse the message created for right hand commands.

        Args:
            servo (Servo): The servo to be moved.
            gesture (str): The gesture that defines the movement.
            servos (list[Servo]): The list that contains servos to be moved.
        """

        print(servo)
        print(gesture)
        if ( servo== "BASE"):
            if (gesture == "SWIPE_LEFT"):
                self.mover.move_servo("pos", servos[0])
            elif (gesture == "SWIPE_RIGHT"):
                self.mover.move_servo("neg", servos[0])
            else:
                pass

        elif(servo == "LOWER_ARM"):
            if(gesture == "UP"):
                self.mover.move_servo("pos", servos[1])
            elif(gesture == "DOWN"):
                self.mover.move_servo("neg", servos[1])
            else:
                pass

        elif(servo == "MIDDLE_ARM"):
            if(gesture == "UP"):
                self.mover.move_servo("pos", servos[2])
            elif(gesture == "DOWN"):
                self.mover.move_servo("neg", servos[2])
            else:
                pass

        elif(servo == "HIGHER_ARM"):
            if(gesture == "UP"):
                self.mover.move_servo("pos", servos[3])
            elif(gesture == "DOWN"):
                self.mover.move_servo("neg", servos[3])
            else:
                pass
            
        elif(servo == "ROTOR"):
        
            if(gesture == "SWIPE_LEFT"):
                self.mover.move_servo("pos", servos[4])
            elif(gesture == "SWIPE_RIGHT"):
                self.mover.move_servo("neg", servos[4])
            else:
                pass
                
        else:
            pass
        
        

    
    def test_parser(self,servo_list: list[Servo])-> None:
        """
        A tester function to test the parser for some messages.

        Args:
            servo_list (list[Servo]): The list that contains servos to be moved.
        """
        
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
                self.parse_input(msg, servo_list)
                time.sleep(0.15)
