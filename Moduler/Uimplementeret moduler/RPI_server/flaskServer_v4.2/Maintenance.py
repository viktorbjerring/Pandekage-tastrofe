from flask import request

class Maintenance:
    def __init__(self):
        self.__panstatus = False
        self.__coolingStatus = False
    def getBatterStatus(self):
        # Brug driver til at finde ud af hvad status på dejen er.
        # Til test er det implementeret med level_alarm filen
        try:
            with open('batter_status.txt', 'r') as systemfile: 
                contents = systemfile.read()
                # If it reads 1, then the indicator is on. 0 means off.
                if(contents == "1"):
                    return 1
                elif(contents == "0"):
                    return 0
                else:
                    # -1 means errors
                    return -1
        except:
            # If unable to open the file
            return -2
    def clearBatterAlarm(self):
        # Brug driver til at skrive, at vi gerne vil slukke indikater
        # Til test er det implementeret med batter_status filen
        try:
            with open('batter_status.txt', 'w') as systemfile: 
                systemfile.write("0") 
            return 1 # Returns 1 if it was able to clear it
        except:
            return 0
    def setCooling(self, init=False):
        if(init == False):
            data = request.get_data(as_text = True)
        else:
            data = init
        if(data == "1"): 
            self.__coolingStatus = True
        elif(data == "0"):
            self.__coolingStatus = False
        else:
            data = "-1"
        return int(data)
    def getCooling(self):
        return(1 if self.__coolingStatus == True else 0)
    def togglePans(self):
        #togglePans()
        self.__panstatus = not self.__panstatus
        return(1 if self.__panstatus == True else 0)
    def panStatus(self):
        value = 1 if self.__panstatus == True else 0
        return value