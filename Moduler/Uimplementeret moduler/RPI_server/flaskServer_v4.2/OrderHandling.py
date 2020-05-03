from PancakeHandler import PancakeHandler
import os
import time

class OrderHandling:
    def __init__(self, orderOverviewObj):
        path = os.getcwd()
        fileName = 'pancakeDone.txt'
        event_handler = PancakeHandler(path, fileName, self.checkIfPancakeIsDone)
        self.__orderOverviewObj = orderOverviewObj
        
    def newOrder(self):
        #do beginPancake() in driver
        print("Told Jesper we made a new order")
        return
    def checkIfPancakeIsDone(self):
        time.sleep(1) # DONT REMOVE. It is necessary or the watchdog will bug!
        with open('pancakeDone.txt', 'r') as systemfile:
                content = systemfile.read()
        if(content[0] == "1"): # if we received a signal that the pancake is done
            self.__orderOverviewObj.oldestPancakeDone()
            with open('pancakeDone.txt', 'w') as systemfile:
                systemfile.write("0")
        return
