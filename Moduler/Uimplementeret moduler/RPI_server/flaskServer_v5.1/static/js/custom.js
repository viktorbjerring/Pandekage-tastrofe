// Ststem status function

// Delivery time function
(function() {

    let delivery_time = document.getElementById("delivery_time");
    let url = document.URL + 'time_estimate/';
    console.log(url);
    fetch(url)
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
    let url = document.URL + 'order_pancake/'
    fetch(url, {
      method: 'POST'
    }).then((response) => {
        return response.text();
    })
    .then((data) => {
        data = data.replace(/['"]+/g, '') // Remove the extra quotation marks
        console.log(data)
        url = document.URL + 'waiting/'
        location.href = url;
    })
    .catch(err => {
        console.log("Error ordering");
        console.error(err);
    });
}

