import random
import os
from flask import json

from OrderOverview import OrderOverview
from Maintenance import Maintenance


class API: # Ansvarlig for alt kommunikation fra IF ind, og sørger for at det returneres korrekt ud.
    def __init__(self):
        self.__orderOverviewObj = OrderOverview()
        self.__maintenanceObj = Maintenance()

        #self.__maintenanceObj.togglePans()
        #self.__maintenanceObj.setCooling(init="1")
    def estimateTime(self):
        return json.dumps(self.__orderOverviewObj.estimateTime())
    def orderPancake(self):
        statusInt = self.__orderOverviewObj.orderPancake()
        if(statusInt == 1):
            status = "Successfully ordered a pancake"
        else:
            status = "Error in processing order: either the IP has made an order already, or a a bug has occured"
        return json.dumps(status)
    def isPancakeDone(self):
        statusInt = self.__orderOverviewObj.isPancakeDone()
        if(statusInt == 1):
            status = "Pancake is ready"
        elif(statusInt == 0):
            status = "Pancake is not ready"
        else:
            status = "Order wasn't made or an error occured"
        print(status)
        return json.dumps(status)
    def getBatterStatus(self):
        statusInt = self.__maintenanceObj.getBatterStatus()
        if(statusInt == 1):
            status = "Dispenser needs batter"
        elif(statusInt == 0):
            status = "Dispenser is ok"
        else:
            status = "An error occured trying to check batter status"
        return json.dumps(status)
    def clearBatterAlarm(self):
        statusInt = self.__maintenanceObj.clearBatterAlarm()
        if(statusInt == 1):
            status = "Batter alarm cleared"
        else:
            status = "Unable to clear batter alarm"
        return json.dumps(status)
    def setCooling(self):
        statusInt = self.__maintenanceObj.setCooling()
        if(statusInt == 1):
            status = "Cooling turned on"
        elif(statusInt == 0):
            status = "Cooling turned off"
        else:
            status = "Unable to set cooling"
        return json.dumps(status)
    def getCooling(self):
        statusInt = self.__maintenanceObj.getCooling()
        if(statusInt == 1):
            status = "Cooling is on"
        elif(statusInt == 0):
            status = "Cooling is off"
        else:
            status = "Unable to get cooling status"
        return json.dumps(status)
    def togglePans(self):
        statusInt = self.__maintenanceObj.togglePans()
        if(statusInt == 1):
            status = "Pans turned on"
        elif(statusInt == 0):
            status = "Pans turned off"
        else:
            status = "Unable to toggle Pans"
        return json.dumps(status)
    def panStatus(self):
        statusInt = self.__maintenanceObj.panStatus()
        if(statusInt == 1):
            status = "Pans are on"
        elif(statusInt == 0):
            status = "Pans are off"
        else:
            status = "Unable to get pan status"
        return json.dumps(status)