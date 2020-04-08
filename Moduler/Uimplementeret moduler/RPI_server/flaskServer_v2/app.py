from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from datetime import datetime
import threading
import time # for sleep
import random # For randomIp
import requests # for being able to make get requests
import os # for forcing exits

app = Flask(__name__) #__name__ = filename

randomIP = False # Debugging

serverURL = "http://127.0.0.1:5000"


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
        return json.dumps(str(wifiObj.isPancakeDone()))

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
        if(self.__idDict.get(maskedIp) == None):
            newDict = {maskedIp: 0}
            self.__idDict.update(newDict)
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
    def isPancakeDone(self):
        id = self.__findUserID()
        if(self.__idDict.get(id)): # If it returns 1 = pancake is done
            self.__idDict.pop(id)
            return 1
        else: # Returns 0 if pancake isn't done OR the ip hasn't made an order
            return 0
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
        try:
            requests.get(serverURL+"/is_server_running/") # Ping /is_server_running/
            return True
        except:
            print("Error on connectToNetwork")
            return False
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
        pass
    def start(self):
        while True:
            if(queueObj.getOrder()):
                print("pancakeBegin()")
            self.pancakeDone()
            time.sleep(5)
    def pancakeDone(self):
        with open('pancakeDone.txt', 'r') as systemfile: 
            contents = systemfile.read()
        if(contents == "1"):
            with open('pancakeDone.txt', 'w') as systemfile:
                systemfile.write("0")
            # Handle the pancake
            order = queueObj.pancakeIsDone()
            wifiObj.pancakeDone(int(order))

class Queue:
    def __init__(self):
        self.__orders = []
        self.__ordersInProduction = 0
    def estimateTime(self): # Returns time
        amountOfOrders = len(self.__orders)
        time = amountOfOrders * 3 + 3 # 3 minutes pr. pancake. + 3 because the order hasn't been put in yet
        return time
 
    def orderPancake(self, id): # A new order has been put
        self.__orders.append(id)
        print("Order pancake called: orders list:")
        print(self.__orders)
        time = len(self.__orders) * 3 # 3 minutes pr. pancake 
        return time

    def getOrder(self): # Gets called when we are about to put an order
        try:
            order = self.__orders[0 + self.__ordersInProduction]
            self.__ordersInProduction = self.__ordersInProduction + 1
            return order
        except:
            print("No orders waiting to be sent")
            return False

    def pancakeIsDone(self):
        print("Queue before popping:")
        print(self.__orders)
        order = self.__orders.pop(0)
        print("Queue after popping:")
        print(self.__orders)
        self.__ordersInProduction = self.__ordersInProduction - 1
        return order

class SystemStartup:
    def __init__(self):
        pass
    def start(self):
        time.sleep(3) # allow the server to boot
        #turnOnCooling()
        #turnOnPans()
        #connectionLED(1)
        status = wifiObj.connectToNetwork()
        if(not status):
            #connectionLED(-1) # maybe it should be 0?
            # Restart server (should this be me?).
            # Can't implement server elegantly... 
            print("Server didn't boot. Quitting")
            os._exit(1) # Hard exit this entire script

class UserIF:
    def __init__(self):
        return
    def indexPage(self):
        return render_template('index.html') #return index page.
    def timeEstimatePage(self):
        return apiObj.estimateTime()
    def orderPancakePage(self):
        return apiObj.orderPancake()
    def isPancakeDone(self):
        return apiObj.isPancakeDone()

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

@app.route('/is_pancake_done/', methods=['GET'])
def is_pancake_done():
    return uiObj.isPancakeDone()

@app.route('/is_server_running/', methods=['GET'])
def is_server_running():
    return "1"

if __name__ == "__main__":
    # Object creation:
    uiObj = UserIF()
    wifiObj = WiFi()
    apiObj = API()
    orderPancakeObj = OrderPancake()
    queueObj = Queue()
    makePancakeObj = MakePancake()
    maintenanceObj = MaintenanceIF()
    systemStartupObj = SystemStartup()


    t1 = threading.Thread(target=makePancakeObj.start, daemon=True).start() # Daemon means shut down thread immediately if we try to stop the proces
    t2 = threading.Thread(target=systemStartupObj.start, daemon=True).start()


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

     # This must be changed to the actual cooking time
    """try:
            with open('queue.txt', 'a+') as systemfile: 
                systemfile.write(str(id) + "\n") #Append
            with open('queue.txt', 'r') as systemfile:
                amountOfOrders = len(systemfile.readlines())
                time = amountOfOrders * 3 # 3 minutes pr. pancake 
                return time 
        except:
            print("Error in Queue: orderPancake()")
            return -1"""