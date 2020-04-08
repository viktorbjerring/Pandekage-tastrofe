// Ststem status function

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
    document.getElementById("order_pancake").addEventListener("click", order_pancake);
}());

function order_pancake() {
    fetch('http://localhost:5000/order_pancake/', {
      method: 'POST'
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

