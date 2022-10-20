from arm import Arm
import time
import machine

def move_arm(inp, arm):
    
    init_angle = arm.get_angle()
    if (inp == "u"):
        arm.set_angle(init_angle + 20)
    elif (inp =="d"):
        arm.set_angle(init_angle -20)
    else:
        arm.set_angle(init_angle)
    
    print(arm.get_angle())



#Initialize arms in a nice position
arm1 = Arm(0, 16, 0)
arm2 = Arm(1, 17, 90)
arm3 = Arm(2, 18, -30)
arm4 = Arm(3, 19, 0)
arm5 = Arm(4, 20, -30)
arm6 = Arm(5, 21, -90)

#Add arms to the list
arm_list = [arm1, arm2, arm3, arm4, arm5, arm6]

while(1):
    arm_num = int(input("Enter the arm number "))
    movement = input("Enter the movement ")
    move_arm(movement, arm_list[arm_num])
