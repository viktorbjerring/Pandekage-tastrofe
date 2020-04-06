from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from datetime import datetime

app = Flask(__name__) #__name__ = filename

# KOMMET TIL ORDERPANCAKE

class API:
    def __init__(self): # Constructor
        pass
    def estimateTime(self): # Returns int
        estimate = wifiObj.estimateTime()
        if(estimate > -1):
            return str(estimate)
        else:
            return Response("Error: Unable to estimate time", status=500, mimetype='application/json')
    def orderPancake(self): # Returns int
        return wifiObj.orderPancake()
    def systemStatus(self): # Returns int
        pass
    def isPancakeDone(self): 
        pass
    def getStatusMsg(self): # Returns char
        # Mangler protokol pt, så vi returnerer bare level alarm
        levelalarm = wifiObj.getLevelAlarm()
        if(levelalarm == 1):
            return "1"
        elif(levelalarm == 0):
            return "0"
        else:
            return "e"
    def setLevelAlarm(self, alarm): # Alarm is bool
        wifiObj.setLevelAlarm(alarm)
        return


class WiFi:
    def __init__(self): # Constructor
        self.__idList = 0 # unordered_map<int, ip>
        self.__levelAlarm = self.getLevelAlarm()

    def estimateTime(self): # Returns int
        try:
            # Husk at ændre filen til den aktuelle!
            with open('time_estimate.txt', 'r') as systemfile: 
                contents = systemfile.read()
            return int(contents)
        except:
            return -1
    def orderPancake(self): # Returns int
        self.__findUserID()
        try:
            with open('pancake_status.txt', 'a+') as systemfile: 
                systemfile.write("Hej\n") #Append
                return json.dumps("Pancake ordered.")
        except:
            return Response("Error: Unable to make order", status=500, mimetype='application/json')
    def sendReady(self, id):
        pass
    def pancakeDone(self, id):
        pass
    def getUserId(self, id):
        pass
    def waitForDone(self, id):
        pass
    def isPancakeDone(self, id):
        pass
    def getLevelAlarm(self): #Returns bool
        try:
            with open('level_alarm.txt', 'r') as systemfile: 
                contents = systemfile.read()
                # If it reads 1, then the indicator is on. 0 means off.
                if(contents == "1"):
                    return True
                else:
                    # -1 means errors
                    return False
        except:
            # If unable to open the file
            return False
    def connectToNetwork(self): # Returns bool
        pass
    def addMsg(self, type): # Type is char
        pass
    def getStatusMsg(self): # Returns char
        pass
    def setLevelAlarm(self, alarm):
        # If alarm is True write "1" in the file. Else write "0"
        temp = "1" if alarm == True else "0"
        with open('level_alarm.txt', 'w') as systemfile: 
            systemfile.write(temp)
        return

    # Private:
    def __findUserID(self): # Private. Returns int
        ip = request.remote_addr
        ipInt = ip.split('.')
        # Convert every number to bits
        # combine numbers
        
        print(type(ipInt))


class UserIF:
    def __init__(self):
        return
    def indexPage(self):
        return render_template('index.html') #return index page.
    def timeEstimatePage(self):
        return apiObj.estimateTime()
    def orderPancakePage(self):
        return apiObj.orderPancake()

class MaintenanceIF:
    def __init__(self):
        return
    def maintenancePage(self):
        status = json.loads(apiObj.getStatusMsg())
        print(status)
        return render_template('maintenance.html', status=status)
    def setAlarmLevelPage(self):
        apiObj.setLevelAlarm(False)
        return
    


# Object creation:
uiObj = UserIF()
wifiObj = WiFi()
apiObj = API()
maintenanceObj = MaintenanceIF()

# flask stuff:

@app.route('/', methods=['GET']) #root directory. POST and GET defines that the directory accepts POST and GET requests
def root_page():
    return uiObj.indexPage()

@app.route('/maintenance/', methods=['GET']) 
def maintenance_page():
    return maintenanceObj.maintenancePage()

@app.route('/set_alarm_level/', methods=['POST'])
def set_alarm_level_page():
    maintenanceObj.setAlarmLevelPage()
    return

@app.route('/time_estimate/', methods=['GET']) 
def time_estimate():
    return uiObj.timeEstimatePage()
    

@app.route('/order_pancake/', methods=['POST'])
def order_pancake():
    return uiObj.orderPancakePage()

if __name__ == "__main__":
    app.run(debug=True)