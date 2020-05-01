from communication import userSpaceCMD
import os
import time
import threading

class OrderHandling:
    def __init__(self, orderOverviewObj):
        self.__orderOverviewObj = orderOverviewObj
        t1 = threading.Thread(target=self.checkIfPancakeIsDone, daemon=True).start() # Daemon means shut down thread immediately if we try to stop the proces
        
    def newOrder(self):
        userSpaceCMD("MAKE_PANCAKE") # Make new pancake
        print("Told system to make a new pancake.")
        return
    def checkIfPancakeIsDone(self):
        while True:
            time.sleep(10) # Do once every 10 seconds
            with open('/dev/pan', 'r') as systemfile: 
                contents = systemfile.read()
                print("Checked if pancake was done - result was:", contents)
            if(contents[0] == "1"):
                self.__orderOverviewObj.oldestPancakeDone()