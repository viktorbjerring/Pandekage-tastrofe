from communication import userSpaceCMD, togglePans
from flask import request

class Maintenance:
    def __init__(self):
        self.__panstatus = False
        self.__coolingStatus = False
    def getBatterStatus(self):
        # Brug driver til at finde ud af hvad status på dejen er.
        # Til test er det implementeret med level_alarm filen
        batterStatus = userSpaceCMD("GET_BATTER_LEVEL")
        print("Asked for batter status:", batterStatus)
        return batterStatus
    def clearBatterAlarm(self):
        response = userSpaceCMD("CLEAR_BATTER_ALARM")
        print("Asked to clear batter alarm:", response)
        return response
    def setCooling(self, init = False):
        if(init == False):
            data = request.get_data(as_text = True)
        else:
            data = "1"
        if(data == "1"): 
            self.__coolingStatus = True
            response = userSpaceCMD("TURN_ON_COOLING") # Returnerer altid 0
            print("Asked to turn on cooling:", response)
        elif(data == "0"):
            self.__coolingStatus = False
            response = userSpaceCMD("TURN_OFF_COOLING") # Returnerer altid 1
            print("Asked to turn off cooling:", response)
        else:
            data = "-1"
        return int(data)
    def getCooling(self):
        return(1 if self.__coolingStatus == True else 0)
    def togglePans(self):
        togglePans()
        self.__panstatus = not self.__panstatus
        return(1 if self.__panstatus == True else 0)
    def panStatus(self):
        value = 1 if self.__panstatus == True else 0
        return value