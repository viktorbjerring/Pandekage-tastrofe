from communication import userSpaceCMD

class BatterStatus:
    def __init__(self):
        pass
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