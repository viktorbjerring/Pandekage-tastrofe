from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
import threading
import random

# Handler stuff
import watchdog.events 
import watchdog.observers 
import time 
import os
mainPath = os.getcwd() # Set the OS working directory (mainPath is used in os)


# Debugging:
randomIP = False # Debugging. Generate random IP instead of actual ones.

# Global vars
timePrPancake = 3 # In minutes for now

# The Flask object
app = Flask(__name__) #__name__ = filename


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
        self.__orders = {} # Could consider using a dataclass instead for readability, but since we implemented this as a dict already, and the performance is a tiny bit better, we stick to it.
        #Orders is structured as following:
        # id: status.
        # Where id is their ip address converted to a number and status is 1 for ready and 0 for not ready.
    def estimateTime(self):
        amountOfOrders = len(self.__orders)
        global timePrPancake
        # TODO: mangler at tage højde for, at vi kan lave to pandekager ad gangen.
        time = amountOfOrders * timePrPancake + timePrPancake # 3 minutes pr. pancake. + 3 because the order hasn't been put in yet
        return time
    def orderPancake(self):
        id = self.__getUserID()
        if(self.__orders.get(id) == None):
            newDict = {id: 0}
            self.__orders.update(newDict)
            orderHandlingObj.newOrder() # Place a new order
            return  1
        else:
            print("Error in processing order: either the IP has made an order already, or a bug has occured")
            return -1
    def oldestPancakeDone(self):
        try:
            index = None # Index of the oldest pancake order in our dict (self.__orders)
            findIndexList = list(self.__orders.values()) # get the values in the dict as a list
            for i in range(len(findIndexList)): # Use the list to loop through it and find the first occurance with status == 0. Save that value in index
                if(findIndexList[i] == 0):
                    index = i
                    break
            idToBeUpdated = list(self.__orders.keys())[index] # Find the corresponding id to the oldest pancake order
            self.__orders.update({idToBeUpdated:1}) # Update the order to have status 1 instead of 0
            print("A pancake was marked as done. The orders are:")
            print(self.__orders)
        except:
            print("Something went terribly wrong (Received signal that a pancake was done, but we haven't made any orders!)")
            return
    def isPancakeDone(self):
        id = self.__getUserID()
        status = self.__orders.get(id)
        if(status == None): # If the key doesn't exist
            status = -1
        
        return self.__orders.get(id)

        

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
        #do beginPancake() in driver
        print("Told Jesper we made a new order")
        return
    def checkIfPancakeIsDone(self):
        time.sleep(1) # DONT REMOVE. It is necessary or the watchdog will bug!
        with open('pancakeDone.txt', 'r') as systemfile:
                content = systemfile.read()
        if(content[0] == "1"): # if we received a signal that the pancake is done
            orderOverviewObj.oldestPancakeDone()
            with open('pancakeDone.txt', 'w') as systemfile:
                systemfile.write("0")
        return

class BatterStatus:
    def __init__(self):
        pass
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
    
    path = r"/home/stud/Pandekage-tastrofe/Moduler/Uimplementeret moduler/RPI_server/flaskServer_v3/"
    fileName = 'pancakeDone.txt'
    event_handler = PancakeHandler(path, fileName)

    # Start API:
    app.run(debug=True,use_reloader = False)