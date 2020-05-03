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
    let body = document.getElementById("body_id");

    let url = document.URL + 'batter_status/';

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
            body.setAttribute("style", "background-color: greenyellow; text-align:center;");
        }
        else if(data === "Dispenser needs batter"){
            header.innerHTML = data;
            body.setAttribute("style","background-color: yellow; text-align:center;");
        }
        else {
            header.innerHTML = "An error occured trying to check batter status"
            body.setAttribute("style","background-color: red; text-align:center;");
        }
    })
    .catch(err => {
        console.log(err);
        header.innerHTML = "An error occured trying to check batter status"
        body.setAttribute("style","background-color: red; text-align:center;");
    });
}

function clearBatterAlarm() {
    let url = document.URL + 'batter_status/';
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
    let url = document.URL + 'pans/';
    fetch(url, {
      method: 'POST',
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


function getPanStatus() {
    let paragraph = document.getElementById("panStatus");
    url = document.URL + 'pans/';
    fetch(url)
    .then((response) => {
        response = response.json()
        return response;
    })
    .then((data) => {
        //console.log("Response:")
        //console.log(data)
        if(data === "Pans are on"){
            paragraph.innerHTML = 'Pans are turned <span style="color:forestgreen;">on</span>.';
        }
        else if(data === "Pans are off"){
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

function turnOnCooling() {
    let url = document.URL + 'cooling/';
    fetch(url, {
      method: 'POST',
      body: JSON.stringify(1)
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
    let url = document.URL + 'cooling/';
    fetch(url, {
      method: 'POST',
      body: JSON.stringify(0)
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

function getCoolingStatus() {
    let paragraph = document.getElementById("coolingStatus");
    url = document.URL + 'cooling/';
    fetch(url)
    .then((response) => {
        response = response.json()
        return response;
    })
    .then((data) => {
        if(data === "Cooling is on"){
            paragraph.innerHTML = 'Cooling is turned <span style="color:forestgreen;">on</span>.';
        }
        else if(data === "Cooling is off"){
            paragraph.innerHTML = 'Cooling is turned <span style="color:red;">off</span>';
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
