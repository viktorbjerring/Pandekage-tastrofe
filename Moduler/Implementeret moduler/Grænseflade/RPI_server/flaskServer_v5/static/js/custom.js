// Ststem status function

// Delivery time function
(function() {

    let delivery_time = document.getElementById("delivery_time");
    fetch('http://localhost:5000/time_estimate/')
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        if(data){
            data = data/60 // Int minutes
            data += " minutes!"
            delivery_time.append(data); // To be replaced by API call. Append whatever the api returns
        }
        else {
            delivery_time.append("Unknown");
        }
    })
    .catch(err => {
        console.log(err);
        delivery_time.append("Unknown");
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
        return response.text();
    })
    .then((data) => {
        data = data.replace(/['"]+/g, '') // Remove the extra quotation marks
        console.log(data)
        location.href = 'http://localhost:5000/waiting/';
    })
    .catch(err => {
        console.log("Error ordering");
        console.error(err);
    });
}

