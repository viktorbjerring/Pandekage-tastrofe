import subprocess
import time

def userSpaceCMD(cmd):
    if(cmd == "MAKE_PANCAKE"):
        print("Running ", cmd)
        cmd = "3"
        returnVal = 0
    elif(cmd == "GET_BATTER_LEVEL"):
        print("Running ", cmd)
        cmd = "4"
        with open('batterLevel.txt', 'r') as systemfile:
            returnVal = int(systemfile.read())
    elif(cmd == "TURN_ON_COOLING"):
        print("Running ", cmd)
        cmd = "5"
        returnVal = 0
    elif(cmd == "TURN_OFF_COOLING"):
        print("Running ", cmd)
        cmd = "6"
        returnVal = 0
    elif(cmd == "CLEAR_BATTER_ALARM"):
        print("Running ", cmd)
        cmd = "7"
        returnVal = 1
    else:
        print("Invalid command")
        return -1
    #userSpaceReturnValue = subprocess.run(["./prog", cmd])
    print("Sent cmd with code: ", cmd)
    return returnVal


def togglePans():
    with open('/dev/pan', 'w') as systemfile: 
            systemfile.write("1")
    print("Toggled pans")