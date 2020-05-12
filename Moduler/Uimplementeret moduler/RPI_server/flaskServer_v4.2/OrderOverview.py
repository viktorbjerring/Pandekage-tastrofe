from datetime import datetime, timedelta
from Order import Order
from OrderHandling import OrderHandling

from flask import request

import random

# Global vars
timePrPancake = 1 # In minutes for now

# Debugging:
randomIP = False # Debugging. Generate random IP instead of actual ones.

class OrderOverview:
    def __init__(self):
        self.__orders = [] # list of Orders
        self.orderHandlingObj = OrderHandling(self)
    def estimateTime(self):
        # TODO: mangler at tage højde for, at vi kan lave to pandekager ad gangen.
        userId = self.__getUserID() # Find the asking users id
        for i in self.__orders: # Check if it already has a pending order
            if(i.id == userId):
                timeRemaining = i.timeDone - datetime.utcnow()
                timeRemaining = timeRemaining.total_seconds()
                return int(timeRemaining) if timeRemaining > 0 else 0 # Return 0 if we have a negative number
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

        print(self.__orders[-1]) # See the new order in console.

        self.orderHandlingObj.newOrder() # Tell orderHandler that we made a new order
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