from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from flask_cors import CORS
from datetime import datetime, timedelta
import threading
import random
import subprocess

# Handler stuff
import watchdog.events 
import watchdog.observers 
import time 
import os
mainPath = os.getcwd() # Set the OS working directory (mainPath is used in os)

from dataclasses import dataclass

# Debugging:
randomIP = False # Debugging. Generate random IP instead of actual ones.

# Global vars
timePrPancake = 3 # In minutes for now

# The Flask object
app = Flask(__name__) #__name__ = filename
CORS(app)

@dataclass
class Order:
    id: int
    timeDone: datetime
    status: int = 0
    timeCreated: datetime = datetime.utcnow()

    def __str__(self):
        done = self.timeDone.strftime("%Y-%m-%dT%H:%M:%S")
        created = self.timeCreated.strftime("%Y-%m-%dT%H:%M:%S")
        return f'Order[id: {self.id}, status: {self.status}, time_created: {created}, time_done: {done}]'

    def __repr__(self):
        return str(self)
    

class API: # Ansvarlig for alt kommunikation fra IF ind, og sørger for at det returneres korrekt ud.
    def __init__(self):
        pass
    def estimateTime(self):
        return json.dumps(orderOverviewObj.estimateTime())
    def orderPancake(self):
        statusInt = orderOverviewObj.orderPancake()
        if(statusInt == 1):
            status = "Successfully ordered a pancake"
        else:
            status = "Error in processing order: either the IP has made an order already, or a a bug has occured"
        return json.dumps(status)
    def isPancakeDone(self):
        statusInt = orderOverviewObj.isPancakeDone()
        if(statusInt == 1):
            status = "Pancake is ready"
        elif(statusInt == 0):
            status = "Pancake is not ready"
        else:
            status = "Order wasn't made or an error occured"
        print(status)
        return json.dumps(status)
    def getBatterStatus(self):
        statusInt = batterStatusObj.getBatterStatus()
        if(statusInt == 1):
            status = "Dispenser needs batter"
        elif(statusInt == 0):
            status = "Dispenser is ok"
        else:
            status = "An error occured trying to check batter status"
        return json.dumps(status)
    def clearBatterAlarm(self):
        statusInt = batterStatusObj.clearBatterAlarm()
        if(statusInt == 1):
            status = "Batter alarm cleared"
        else:
            status = "Unable to clear batter alarm"
        return json.dumps(status)
    
class OrderOverview:
    def __init__(self):
        self.__orders = [] # list of Orders
    def estimateTime(self):
        # TODO: mangler at tage højde for, at vi kan lave to pandekager ad gangen.
        userId = self.__getUserID() # Find the asking users id
        for i in self.__orders: # Check if it already has a pending order
            if(i.id == userId):
                timeRemaining = i.timeDone - datetime.utcnow()
                return int(timeRemaining.total_seconds())
        # Else print expected to for a new order        
        global timePrPancake
        time = timePrPancake * (len(self.__orders) + 1) * 60 # +1 because the order hasn't been put in yet. * 60 because we want it in seconds
        return time
    def orderPancake(self):
        newId = self.__getUserID() # Find id

        for i in self.__orders: # Check if it already has a pending order
            if(i.id == newId):
                print("Error in processing order: either the IP has made an order already, or a bug has occured")
                return -1

        timeDone = datetime.utcnow() + timedelta(seconds = self.estimateTime()) # Get the time when it will be done
        newOrder = Order(newId, timeDone) # Make the new order object
        self.__orders.append(newOrder)

        #print(self.__orders[-1]) # See the new order in console.

        orderHandlingObj.newOrder() # Tell orderHandler that we made a new order
        return 1
    def oldestPancakeDone(self):
        for i in self.__orders:
            if(i.status == 0):
                i.status = 1
                print("A pancake was marked as done. The orders are:")
                print(self.__orders)
                return
        print("Something went terribly wrong (Received signal that a pancake was done, but we haven't made any orders!)")
        return
    def isPancakeDone(self):
        askingId = self.__getUserID() # Get his ID
        for i in self.__orders: # Return the status for the asking guys id
            if(i.id == askingId):
                status = i.status
                if(i.status == 1): # The person asked about his pancake, and it is done, remove it from the list of orders
                    self.__orders.remove(i)
                return status
        return -1 # return -1 if order doesnt exist        

    #PRIVATE:
    def __getUserID(self):
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

class OrderHandling:
    def __init__(self):
        pass
    def newOrder(self):
        makePancakeCode = "3"
        returnValue = subprocess.run(["./prog", makePancakeCode])
        print("Told Jesper we made a new order:")
        print(returnValue.returncode)
        return
    def checkIfPancakeIsDone(self):
        time.sleep(1) # DONT REMOVE. It is necessary or the watchdog will bug!
        with open('/dev/pan', 'r') as systemfile: 
            contents = systemfile.read()
            print("Checked if pancake was done - result was:")
            print(contents)
        if(contents[0] == "1"):
            orderOverviewObj.oldestPancakeDone()
        return

class BatterStatus:
    def __init__(self):
        pass
    def getBatterStatus(self):
        # Brug driver til at finde ud af hvad status på dejen er.
        # Til test er det implementeret med level_alarm filen
        batterStatusCode = "4"
        returnValue = subprocess.run(["./prog", batterStatusCode])
        print("Asked Jesper for batter status:")
        print(returnValue.returncode)
        return returnValue.returncode
    def clearBatterAlarm(self):
        # Brug driver til at skrive, at vi gerne vil slukke indikater
        # Til test er det implementeret med batter_status filen
        try:
            with open('batter_status.txt', 'w') as systemfile: 
                systemfile.write("0") 
            return 1 # Returns 1 if it was able to clear it
        except:
            return 0

class SystemStatus:
    def __init__(self):
        pass

class PancakeHandler(watchdog.events.PatternMatchingEventHandler): 
    def __init__(self, src, fileName): 
    # Set the patterns for PatternMatchingEventHandler 
        self.__fileName = fileName
        self.__lastModified = 0
        fileNamePath = '*'+fileName # Necessary to make it a pattern...
        observer = watchdog.observers.Observer()
        observer.schedule(self, path=src, recursive=True) 
        observer.start()
        watchdog.events.PatternMatchingEventHandler.__init__(self, patterns=[fileNamePath], 
                                                            ignore_directories=True, case_sensitive=False) 

    def on_modified(self, event): 
        statbuf = os.stat(self.__fileName)
        newEvent = statbuf.st_mtime
        if((newEvent - self.__lastModified) > 0.5):
            #print("Watchdog received modified event - % s." % event.src_path)
            orderHandlingObj.checkIfPancakeIsDone()
                    
        self.__lastModified = newEvent

# flask stuff:

#---------------------------UserIF:
@app.route('/', methods=['GET']) #root directory. POST and GET defines that the directory accepts POST and GET requests
def root_page():
    return render_template('index.html') #return index page.

@app.route('/time_estimate/', methods=['GET']) 
def time_estimate():
    return apiObj.estimateTime()
    
@app.route('/order_pancake/', methods=['POST'])
def order_pancake():
    return apiObj.orderPancake()

@app.route('/is_pancake_done/', methods=['GET'])
def is_pancake_done():
    return apiObj.isPancakeDone()

@app.route('/waiting/', methods=['GET'])
def waiting():
    return render_template('waiting.html')
#---------------------------UserIF END


#---------------------------MaintenanceIF:
@app.route('/maintenance/', methods=['GET']) 
def maintenance_page():
    return render_template('maintenance.html')

@app.route('/maintenance/get_batter_status/', methods=['GET'])
def get_batter_status():
    return apiObj.getBatterStatus()

@app.route('/maintenance/clear_batter_alarm/', methods=['POST'])
def set_batter_alarm():
    return apiObj.clearBatterAlarm()


#---------------------------MaintenanceIF END

#---------------------------PING:
@app.route('/is_server_running/', methods=['GET'])
def is_server_running():
    return "1"
#---------------------------PING END

if __name__ == "__main__":
    # Object creation:
    apiObj = API()
    orderOverviewObj = OrderOverview()
    orderHandlingObj = OrderHandling()
    batterStatusObj = BatterStatus()

    turnOnCoolingCode = "5"
    returnValue = subprocess.run(["./prog", turnOnCoolingCode])
    print("Asked Jesper to turn on cooling:")
    print(returnValue.returncode)

    #path = r"/home/morten/pandemix/Moduler/Uimplementeret moduler/RPI_server/flaskServer_v4/" # For testing on mortens PC
    path = r"/dev/"
    fileName = 'pan'
    event_handler = PancakeHandler(path, fileName)

    # Start API:
    app.run(debug=True,use_reloader = False, host="0.0.0.0")