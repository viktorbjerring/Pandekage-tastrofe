import subprocess
import time

def userSpaceCMD(cmd):
    if(cmd == "MAKE_PANCAKE"):
        print("Running ", cmd)
        cmd = "3"
    elif(cmd == "GET_BATTER_LEVEL"):
        print("Running ", cmd)
        cmd = "4"
    elif(cmd == "TURN_ON_COOLING"):
        print("Running ", cmd)
        cmd = "5"
    elif(cmd == "TURN_OFF_COOLING"):
        print("Running ", cmd)
        cmd = "6"
    elif(cmd == "CLEAR_BATTER_ALARM"):
        print("Running ", cmd)
        cmd = "7"
    else:
        print("Invalid command")
        return -1
    userSpaceReturnValue = subprocess.run(["./prog", cmd])
    print("Sent userspace cmd with code: ", cmd)
    return userSpaceReturnValue.returncode


def togglePans():
    with open('/dev/pan', 'w') as systemfile: 
            systemfile.write("1")
            print("Turned on the pans.")