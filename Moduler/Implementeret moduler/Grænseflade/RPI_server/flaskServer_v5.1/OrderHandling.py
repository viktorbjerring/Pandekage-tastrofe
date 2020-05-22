from PancakeHandler import PancakeHandler
from communication import userSpaceCMD
import os
import time

class OrderHandling:
    def __init__(self, orderOverviewObj):
        path = r"/dev/"
        fileName = 'pan'
        event_handler = PancakeHandler(path, fileName, self.checkIfPancakeIsDone)
        self.__orderOverviewObj = orderOverviewObj
        
    def newOrder(self):
        userSpaceCMD("MAKE_PANCAKE") # Make new pancake
        print("Told system to make a new pancake.")
        return
    def checkIfPancakeIsDone(self):
        time.sleep(1) # DONT REMOVE. It is necessary or the watchdog will bug!
        with open('/dev/pan', 'r') as systemfile: 
            contents = systemfile.read()
            print("Checked if pancake was done - result was:")
            print(contents)
        if(contents[0] == "1"):
            self.__orderOverviewObj.oldestPancakeDone()
        return
