// Setting the width of the progressback and success image to the same as the pancake_button
$(document).ready(function() {
    
});

// Check if order was made:
(function() {
    
    let url = document.URL + 'is_pancake_done';
    fetch(url)
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        if(data == "Order wasn't made or an error occured"){
            togglePancakeButton(1);
        } else {
            togglePancakeButton(2);
        }
    });
}());

function togglePancakeButton(state) {
    let pancakeButtonDiv = document.getElementById("pancake-button");
    let waitingDiv = document.getElementById("waiting");
    let successDiv = document.getElementById("success");
    switch(state){
        case 1: 
            waitingDiv.setAttribute('style', 'display:none;');
            successDiv.setAttribute('style', 'display:none;');
            pancakeButtonDiv.setAttribute('style', 'display:block;');
            break;
        case 2: // Order counting down
            pancakeButtonDiv.setAttribute('style', 'display:none;');
            successDiv.setAttribute('style', 'display:none;');
            waitingDiv.setAttribute('style', 'display:block;');
            $(".waiting").css({ // Set it to same size as pancake-button. Must be after setAttribute
                'width': ($(".pancake-button").width() + 'px')
            });
            startCountdown(); // From waiting.js
            break;
        case 3: // Transition between state 2 and 1
            pancakeButtonDiv.setAttribute('style', 'display:none;');
            waitingDiv.setAttribute('style', 'display:none;');
            successDiv.setAttribute('style', 'display:block;');
            $(".success").css({ // Set it to same size as pancake-button. Must be after setAttribute
                'width': ($(".pancake-button").width() + 'px')
            });
            setTimeout(() => {
                togglePancakeButton(1);
            }, 3000);
            break;
    }
    return;
}


// Delivery time function
(function() {
    let delivery_time = document.getElementById("delivery_time");
    let url = document.URL + 'time_estimate/';
    fetch(url)
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        if(Number.isInteger(data)){
            data = Math.ceil(data/60) // Int minutes
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
        togglePancakeButton(2);
        //url = document.URL + 'waiting/'
        //location.href = url;
    })
    .catch(err => {
        console.log("Error ordering");
        console.error(err);
    });
}

