// Ststem status function

let userID; 

(function() {

    let system_status = document.getElementById("system_status");
    fetch('http://localhost:5000/system_status/')
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        //console.log(data);
        if(data){
            system_status.append(data); // To be replaced by API call. Append whatever the api returns
        }
        else {
            system_status.append("Unknown");
        }
    })
    .catch(err => {
        console.error('Error:', err);
        system_status.append("Unknown");
    });

}());

// Delivery time function
(function() {

    let delivery_time = document.getElementById("delivery_time");
    fetch('http://localhost:5000/time_estimate/')
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        //console.log(data);
        if(data){
            data += " minutes!"
            delivery_time.append(data); // To be replaced by API call. Append whatever the api returns
        }
        else {
            delivery_time.append("Unknown");
        }
    })
    .catch(err => {
        console.log(err);
        system_status.append("Unknown");
    });
}());

// Order pancake function
(function() {
    userID = Date.now();
    document.getElementById("order_pancake").addEventListener("click", order_pancake);
}());

function order_pancake() {
    fetch('http://localhost:5000/order_pancake/', {
      method: 'POST',
      body: JSON.stringify(userID)  
    }).then((response) => {
        return response.json();
    })
    .then((data) => {
        console.log("Succes: ",data);
    })
    .catch(err => {
        console.error(err);
    });
}

