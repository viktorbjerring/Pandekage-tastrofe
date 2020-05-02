import random
import os
from flask import json

from OrderOverview import OrderOverview
from BatterStatus import BatterStatus
from communication import userSpaceCMD, togglePans


class API: # Ansvarlig for alt kommunikation fra IF ind, og sørger for at det returneres korrekt ud.
    def __init__(self):
        #userSpaceCMD("TURN_ON_COOLING") # Send command to turn on cooling
        
        togglePans()

        self.orderOverviewObj = OrderOverview()
        self.batterStatusObj = BatterStatus()
        self.__panstatus = False
    def estimateTime(self):
        return json.dumps(self.orderOverviewObj.estimateTime())
    def orderPancake(self):
        statusInt = self.orderOverviewObj.orderPancake()
        if(statusInt == 1):
            status = "Successfully ordered a pancake"
        else:
            status = "Error in processing order: either the IP has made an order already, or a a bug has occured"
        return json.dumps(status)
    def isPancakeDone(self):
        statusInt = self.orderOverviewObj.isPancakeDone()
        if(statusInt == 1):
            status = "Pancake is ready"
        elif(statusInt == 0):
            status = "Pancake is not ready"
        else:
            status = "Order wasn't made or an error occured"
        print(status)
        return json.dumps(status)
    def getBatterStatus(self):
        statusInt = self.batterStatusObj.getBatterStatus()
        if(statusInt == 1):
            status = "Dispenser needs batter"
        elif(statusInt == 0):
            status = "Dispenser is ok"
        else:
            status = "An error occured trying to check batter status"
        return json.dumps(status)
    def clearBatterAlarm(self):
        statusInt = self.batterStatusObj.clearBatterAlarm()
        if(statusInt == 1):
            status = "Batter alarm cleared"
        else:
            status = "Unable to clear batter alarm"
        return json.dumps(status)
    def turnCoolingOn(self):
        userSpaceCMD("TURN_ON_COOLING") # Send command to turn on cooling
        return json.dumps("1")
    def turnCoolingOff(self):
        userSpaceCMD("TURN_OFF_COOLING") # Send command to turn on cooling
        return json.dumps("1")
    def togglePans(self):
        togglePans()
        self.__panstatus = not self.__panstatus
        value = "1" if self.__panstatus == True else "0"
        return json.dumps(value)