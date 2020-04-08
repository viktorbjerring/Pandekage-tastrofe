from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from datetime import datetime
from Watchdog import Handler
import threading
import time # for sleep
import random # For randomIp
import requests # for being able to make get requests
import os # for forcing exits

app = Flask(__name__) #__name__ = filename

randomIP = False # Debugging

serverURL = "http://127.0.0.1:5000"


# Note: Hvis vi skal lave en pandekage, skal vi køre programmet med et 4-tal

class API: # Ansvarlig for alt kommunikation fra IF ind, og sørger for at det returneres korrekt ud.
    def __init(self):
        pass

class OrderOverview:
    def __init(self):
        self.__orders = {}

class OrderHandling:
    def __init(self):
        pass

class BatterStatus:
    def __init(self):
        pass

# flask stuff:

#---------------------------UserIF:
@app.route('/', methods=['GET']) #root directory. POST and GET defines that the directory accepts POST and GET requests
def root_page():
    return render_template('index.html') #return index page.


@app.route('/time_estimate/', methods=['GET']) 
def time_estimate():
    return "pass"
    
@app.route('/order_pancake/', methods=['POST'])
def order_pancake():
    return "pass"

@app.route('/is_pancake_done/', methods=['GET'])
def is_pancake_done():
    return "pass"
#---------------------------UserIF END


#---------------------------MaintenanceIF:
@app.route('/maintenance/', methods=['GET']) 
def maintenance_page():
    status = json.loads("pass")
    return render_template('maintenance.html', status=status)

@app.route('/set_alarm_level/', methods=['POST'])
def set_alarm_level_page():
    return
#---------------------------MaintenanceIF END

#---------------------------PING:
@app.route('/is_server_running/', methods=['GET'])
def is_server_running():
    return "1"
#---------------------------PING END

if __name__ == "__main__":
    # Object creation:
    


    # Start API:
    app.run(debug=True,use_reloader = False)