from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from datetime import datetime
from Watchdog import Handler
import threading
import time
import random

app = Flask(__name__) #__name__ = filename

randomIP = True # Debugging

# Note: Hvis vi skal lave en pandekage, skal vi køre programmet med et 4-tal

# Du er kommet til at implementere nederste halvdel af figur 8.5

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
        time = wifiObj.orderPancake()
        return json.dumps(str(time))
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
        self.__idDict = {}
        self.__levelAlarm = self.getLevelAlarm()
        # Flush old queue
        with open('queue.txt', 'w') as systemfile: 
            systemfile.write("")

    def estimateTime(self): # Returns int
        return orderPancakeObj.estimateTime()
    def orderPancake(self): # Returns int
        maskedIp = self.__findUserID()
        if(maskedIp not in self.__idDict.values()):
            newDict = {maskedIp: 0}
            self.__idDict.update(newDict)
            print(self.__idDict)
            return orderPancakeObj.orderPancake(maskedIp)
        else:
            print("Error in processing order: either the IP has made an order already, or a bug has occured")
            return -1
    def sendReady(self, id):
        pass
    def pancakeDone(self, id):
        newDict = {id: 1}
        self.__idDict.update(newDict)
        print("A pancake is done:")
        print(newDict)

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
        ipStr = request.remote_addr # Get ip
        ipStrList = ipStr.split('.') # Seperate by .


        # Take each 8-bit byte from the IP and return it as one 32-bit byte.
        # Nb the ip has been changed to the following:
        # If the OG ip was 192.168.0.1
        # The new ip would be 1.0.168.192 if it was to be deconstructed.
        ipInt = 0
        for i in range(len(ipStrList)):
            ipInt = ipInt + (int(ipStrList[i]) << i*8)
        global randomIP
        if(randomIP): # For debugging
            return random.randint(10000,999999)
        return ipInt


class OrderPancake:
    def __init__(self):
        pass
    def estimateTime(self): # Returns time
        return queueObj.estimateTime()
    def orderPancake(self, id):
        return queueObj.orderPancake(id)

class MakePancake:
    def __init__(self):
        self.__ordersInProduction = []
    def start(self):
        while True:
            with open('queue.txt', 'r') as systemfile: 
                contents = systemfile.read()
                if(contents):
                    self.__ordersInProduction.append(queueObj.getOrder())
                    print("pancakeBegin()")
                    # use i2c program to say pancakeBegin()
            self.pancakeDone()
            time.sleep(5)
    def pancakeDone(self):
        with open('pancakeDone.txt', 'r') as systemfile: 
            contents = systemfile.read()
        if(contents == "1"):
            with open('pancakeDone.txt', 'w') as systemfile:
                systemfile.write("0")
            # Handle the pancake
            id = self.__ordersInProduction.pop(0)
            wifiObj.pancakeDone(int(id))

class Queue:
    def __init__(self):
        pass
    def estimateTime(self): # Returns time
        try:
            with open('queue.txt', 'r') as systemfile:
                amountOfOrders = len(systemfile.readlines())
                time = amountOfOrders * 3 # 3 minutes pr. pancake 
                return int(time)
        except:
            print("Error in Queue: orderPancake()")
            return -1
    def orderPancake(self, id):
        # This must be changed to the actual cooking time
        try:
            with open('queue.txt', 'a+') as systemfile: 
                systemfile.write(str(id) + "\n") #Append
            with open('queue.txt', 'r') as systemfile:
                amountOfOrders = len(systemfile.readlines())
                time = amountOfOrders * 3 # 3 minutes pr. pancake thread.start_new_thread(flaskThread,())
                return time 
        except:
            print("Error in Queue: orderPancake()")
            return -1

    def getOrder(self):
        with open('queue.txt', 'r') as systemfile:
            contents = systemfile.read().splitlines(True)
        with open('queue.txt', 'w') as systemfile:
            systemfile.writelines(contents[1:]) # write everything except the oldest order
        return contents[0]


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
    # Object creation:
    uiObj = UserIF()
    wifiObj = WiFi()
    apiObj = API()
    orderPancakeObj = OrderPancake()
    queueObj = Queue()
    makePancakeObj = MakePancake()
    maintenanceObj = MaintenanceIF()

    path = r"/home/stud/Pandekage-tastrofe/Moduler/Uimplementeret moduler/RPI_server/flaskServer_v2/"
    fileName = 'panController.txt'
    event_handler = Handler(path, fileName)

    x = threading.Thread(target=makePancakeObj.start, daemon=True).start() # Daemon means shut down thread immediately if we try to stop the proces

    # Start API:
    app.run(debug=True,use_reloader = False)


    # Old (for documentation):
    """def __findUserID(self): # Private. Returns int
        ipStr = request.remote_addr # Get ip
        ipStrList = ipStr.split('.') # Seperate by .
        ipStr = 0
        for i in ipStrList:
            ipStr = ipStr + i

        return int(ipStr)"""
    
    """try:
            with open('pancake_status.txt', 'a+') as systemfile: 
                systemfile.write("Hej\n") #Append
                return json.dumps("Pancake ordered.")
        except:
            return Response("Error: Unable to make order", status=500, mimetype='application/json')"""

    """try:
            # Husk at ændre filen til den aktuelle!
            with open('time_estimate.txt', 'r') as systemfile: 
                contents = systemfile.read()
            return int(contents)
        except:
            return -1"""