// Order pancake function
(function() {
    document.getElementById("clearBatterAlarm").addEventListener("click", clearBatterAlarm);
    getBatterStatus()
}());

// Make API call to check status every 10 seconds
(function() {
    setInterval(function(){
        getBatterStatus()
    }, 10000);
}());

// Change page based on what data we fetch
function getBatterStatus() {

    let header = document.getElementById("batter_status");
    let button = document.getElementById("clearBatterAlarm");
    let body = document.getElementById("body_id");

    fetch('http://localhost:5000/maintenance/get_batter_status/')
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
    fetch('http://localhost:5000/maintenance/clear_batter_alarm/', {
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

