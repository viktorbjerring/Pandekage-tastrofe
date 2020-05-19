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

@app.route('/pancake/', methods=['POST','GET'])
def pancake():
    if(request.method == 'POST'):
        return apiObj.orderPancake()
    else:
        return apiObj.isPancakeDone()
#---------------------------UserIF END


#---------------------------MaintenanceIF:
@app.route('/maintenance/', methods=['GET']) 
def maintenance_page():
    return render_template('maintenance.html')

@app.route('/maintenance/batter_status/', methods=['POST','GET'])
def batterStatus():
    if(request.method == 'POST'):
        return apiObj.clearBatterAlarm()
    else:
        return apiObj.getBatterStatus()


@app.route('/maintenance/cooling/', methods=['POST','GET'])
def cooling():
    if(request.method == 'POST'):
        return apiObj.setCooling()
    else:
        return apiObj.getCooling()

@app.route('/maintenance/pans/', methods=['POST','GET'])
def pans():
    if(request.method == 'POST'):
        return apiObj.togglePans()
    else:
        return apiObj.panStatus()
#---------------------------MaintenanceIF END


if __name__ == "__main__":
    # Object creation:
    apiObj = API()

    # Start API:
    app.run(debug=True,use_reloader = False, host='0.0.0.0')
