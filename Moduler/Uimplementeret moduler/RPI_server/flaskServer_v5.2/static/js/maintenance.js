// Order pancake function
(function() {
    document.getElementById("clearBatterAlarm").addEventListener("click", clearBatterAlarm);
    document.getElementById("togglePans").addEventListener("click", togglePans);
    document.getElementById("turnOnCooling").addEventListener("click", turnOnCooling);
    document.getElementById("turnOffCooling").addEventListener("click", turnOffCooling);
    getBatterStatus();
    getPanStatus();
    getCoolingStatus();
}());

// Make API call to check status every 10 seconds
(function() {
    setInterval(function(){
        getBatterStatus();
    }, 10000);
}());

// Change page based on what data we fetch
function getBatterStatus() {

    let header = document.getElementById("batter_status");
    let button = document.getElementById("clearBatterAlarm");
    let body = document.getElementById("body_id");

    let url = document.URL + 'get_batter_status/';

    fetch(url)
    .then((response) => {
        response = response.json()
        return response;
    })
    .then((data) => {
        //console.log("Response:")
        //console.log(data)
        if(data === "Dispenser is ok"){
            header.innerHTML = data;
            button.setAttribute("style", "display:none;");
            body.setAttribute("style", "background-color: greenyellow; text-align:center;");
        }
        else if(data === "Dispenser needs batter"){
            header.innerHTML = data;
            body.setAttribute("style","background-color: yellow; text-align:center;");
            button.setAttribute("style", "display:inline-block;");
        }
        else {
            header.innerHTML = "An error occured trying to check batter status"
            button.setAttribute("style", "display:none;");
            body.setAttribute("style","background-color: red; text-align:center;");
        }
    })
    .catch(err => {
        console.log(err);
        header.innerHTML = "An error occured trying to check batter status"
        button.setAttribute("style", "display:none;");
        body.setAttribute("style","background-color: red; text-align:center;");
    });
}

function clearBatterAlarm() {
    let url = document.URL + 'clear_batter_alarm/';
    fetch(url, {
      method: 'POST'
    }).then((response) => {
        return response.text();
    })
    .then((data) => {
        //console.log(data)
        getBatterStatus() // Check new status
    })
    .catch(err => {
        console.error(err);
    });
} 

function togglePans() {
    let url = document.URL + 'toggle_pans/';
    fetch(url, {
      method: 'POST'
    }).then((response) => {
        return response.text();
    })
    .then((data) => {
        console.log(data);
        getPanStatus();
    })
    .catch(err => {
        console.error(err);
    });
}

function turnOnCooling() {
    let url = document.URL + 'cooling_on/';
    fetch(url, {
      method: 'POST'
    }).then((response) => {
        return response.text();
    })
    .then((data) => {
        console.log(data)
        getCoolingStatus();
    })
    .catch(err => {
        console.error(err);
    });
}

function turnOffCooling() {
    let url = document.URL + 'cooling_off/';
    fetch(url, {
      method: 'POST'
    }).then((response) => {
        return response.text();
    })
    .then((data) => {
        console.log(data)
        getCoolingStatus();
    })
    .catch(err => {
        console.error(err);
    });
}

function getPanStatus() {
    let paragraph = document.getElementById("panStatus");
    url = document.URL + 'pan_status/';
    fetch(url)
    .then((response) => {
        response = response.json()
        return response;
    })
    .then((data) => {
        //console.log("Response:")
        //console.log(data)
        if(data === "1"){
            paragraph.innerHTML = 'Pans are turned <span style="color:forestgreen;">on</span>.';
        }
        else if(data === "0"){
            paragraph.innerHTML = 'Pans are turned <span style="color:red;">off</span>.';
        }
        else {
            paragraph.innerHTML = "An error occured trying to check for pan status"
        }
    })
    .catch(err => {
        console.log(err);
        paragraph.innerHTML = "An error occured trying to check for pan status";
    });
}

function getCoolingStatus() {
    let paragraph = document.getElementById("coolingStatus");
    url = document.URL + 'cooling_status/';
    fetch(url)
    .then((response) => {
        response = response.json()
        return response;
    })
    .then((data) => {
        //console.log("Response:")
        //console.log(data)
        if(data === "1"){
            paragraph.innerHTML = "Cooling is turned on";
        }
        else if(data === "0"){
            paragraph.innerHTML = "Cooling is turned off";
        }
        else {
            paragraph.innerHTML = "An error occured trying to check for cooling status"
        }
    })
    .catch(err => {
        console.log(err);
        paragraph.innerHTML = "An error occured trying to check for cooling status";
    });
}
