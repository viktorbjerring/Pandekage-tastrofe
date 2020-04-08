from flask import Flask, render_template, url_for, request, redirect, json, jsonify, Response
from datetime import datetime

app = Flask(__name__) #__name__ = filename

@app.route('/', methods=['GET']) #root directory. POST and GET defines that the directory accepts POST and GET requests
def index():
    return render_template('index.html') #return index page.

@app.route('/system_status/', methods=['GET']) 
def system_status():
    # Husk at ændre filen til den aktuelle!
    try:
        with open('system_status.txt', 'r') as systemfile: 
            contents = systemfile.read()

        return json.dumps(contents)
    except:
        return Response("Error: Unable to read system_status file", status=500, mimetype='application/json')

@app.route('/time_estimate/', methods=['GET']) 
def time_estimate():
    try:
        # Husk at ændre filen til den aktuelle!
        with open('time_estimate.txt', 'r') as systemfile: 
            contents = systemfile.read()

        return json.dumps(contents)
    except:
        return Response("Error: Unable to read time_estimate file", status=500, mimetype='application/json')

@app.route('/order_pancake/', methods=['POST', 'GET'])
def order_pancake():
    if request.method == 'POST':
        try:
            with open('pancake_status.txt', 'a+') as systemfile: 
                systemfile.write("Hej\n") #Append
                return json.dumps("Pancake ordered.")
        except:
            return Response("Error: Unable to make order", status=500, mimetype='application/json')
    else:
        return redirect("/", code=400)

if __name__ == "__main__":
    app.run(debug=True)