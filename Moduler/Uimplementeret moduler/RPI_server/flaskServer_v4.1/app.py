from API import API
from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from flask_cors import CORS

# The Flask object
app = Flask(__name__) #__name__ = filename
CORS(app)

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

    # Start API:
    app.run(debug=True,use_reloader = False, host='0.0.0.0')
