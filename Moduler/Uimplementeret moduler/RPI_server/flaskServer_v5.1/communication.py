import subprocess

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
    else:
        print("Invalid command")
        return -1
    userSpaceReturnValue = subprocess.run(["./prog", cmd])
    print("Sent userspace cmd with code: ", cmd)
    return userSpaceReturnValue.returncode