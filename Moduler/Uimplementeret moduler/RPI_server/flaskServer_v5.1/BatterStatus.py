from communication import userSpaceCMD

class BatterStatus:
    def __init__(self):
        pass
    def getBatterStatus(self):
        # Brug driver til at finde ud af hvad status på dejen er.
        # Til test er det implementeret med level_alarm filen
        batterStatus = userSpaceCMD("GET_BATTER_LEVEL")
        print("Asked for batter status:")
        print(batterStatus)
        return batterStatus
    def clearBatterAlarm(self):
        # Brug driver til at skrive, at vi gerne vil slukke indikater
        # Til test er det implementeret med batter_status filen
        try:
            with open('batter_status.txt', 'w') as systemfile: 
                systemfile.write("0") 
            return 1 # Returns 1 if it was able to clear it
        except:
            return 0