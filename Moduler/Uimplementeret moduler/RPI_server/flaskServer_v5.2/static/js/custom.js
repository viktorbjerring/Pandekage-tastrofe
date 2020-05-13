// Check if order was made:
(function() {
    let url = document.URL + 'is_pancake_done/';
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

async function togglePancakeButton(state) {
    let pancakeButtonDiv = document.getElementById("pancake-button");
    let waitingDiv = document.getElementById("waiting");
    let successDiv = document.getElementById("success");
    let almostSuccessDiv = document.getElementById("almost-success");
    switch(state){
        case 1: 
            waitingDiv.setAttribute('style', 'display:none;');
            successDiv.setAttribute('style', 'display:none; position:absolute');
            almostSuccessDiv.setAttribute('style', 'display:none; position:absolute;');
            pancakeButtonDiv.setAttribute('style', 'display:block; position:relative;');
            break;
        case 2: // Order counting down
            successDiv.setAttribute('style', 'display:none; position:absolute');
            almostSuccessDiv.setAttribute('style', 'display:none; position:absolute;');
            pancakeButtonDiv.setAttribute('style', 'display:none; position:relative;');
            waitingDiv.setAttribute('style', 'display:block;');
            $(".waiting").css({ // Set it to same size as pancake-button. Must be after setAttribute
                'width': ($(".pancake-button").width() + 'px')
            });
            startCountdown(); // From waiting.js
            break;
        case 3: // Transition between state 2 and 1
            waitingDiv.setAttribute('style', 'display:none;');
            almostSuccessDiv.setAttribute('style', 'display:none; position:absolute;');
            pancakeButtonDiv.setAttribute('style', 'display:block; position:relative; opacity: 1;');
            successDiv.setAttribute('style', 'display:block; position:absolute');
            $(".success").css({ // Set it to same size as pancake-button. Must be after setAttribute
                'height': ($(".pancake-button").height() + 'px')
            });
            document.getElementById("action_text").innerHTML = "IT'S PANCAKE TIME!"
            $("#success").fadeOut(5000);
            setTimeout(() => {
                document.getElementById("action_text").innerHTML = "CLICK THE PANCAKE TO ORDER!"
                document.getElementById("delivery_time").setAttribute('style', 'display:block;')
                deliveryTime();
                togglePancakeButton(1);
            }, 5000);
            break;
        case 4: // If timer is done but pancake isnt
            let url = document.URL + 'is_pancake_done/';
            fetch(url)
            .then((response) => {
                return response.json();
            })
            .then((data) => {
                waitingDiv.setAttribute('style', 'display:none;');
                pancakeButtonDiv.setAttribute('style', 'display:block; position:relative; opacity: 0;');
                successDiv.setAttribute('style', 'display:none; position:absolute');
                almostSuccessDiv.setAttribute('style', 'display:block; position:absolute;');
                $(".almost-success").css({ // Set it to same size as pancake-button. Must be after setAttribute
                    'height': ($(".pancake-button").height() + 'px')
                });
                if(data == "Pancake is ready"){
                    togglePancakeButton(3);
                } else {
                    $(".success").css({ // Set it to same size as pancake-button. Must be after setAttribute
                        'height': ($(".pancake-button").height() + 'px')
                    });
                    document.getElementById("action_text").innerHTML = "YOUR PANCAKE IS READY SOON!"
                    document.getElementById("delivery_time").setAttribute('style', 'display:none;')
                    setTimeout(() => {
                        togglePancakeButton(4);
                    }, 5000);
                }
            });
        }
    return;
}

// Delivery time function
(function() {
    deliveryTime(); // Check delivery time now and every 10th second
    let interval = setInterval(function(){
        deliveryTime();
    }, 10000);
}());

async function deliveryTime() {
    let delivery_time = document.getElementById("delivery_time");
    let url = document.URL + 'time_estimate/';
    fetch(url)
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        if(Number.isInteger(data)){
            data = Math.ceil(data/60); // Int minutes
            data += " minutes!";
            delivery_time.innerHTML = ("Expected delivery time:<br>");
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
}

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

// Get confirmation-time in modal
$('#confirmation').on('show.bs.modal', function (event) {
    let paragraph = document.getElementById("confirm_delivery_time");
    paragraph.innerHTML= "Expected delivery time:<br>";
    let url = document.URL + 'time_estimate/';
    fetch(url)
    .then((response) => {
        return response.json();
    })
    .then((data) => {
        if(Number.isInteger(data)){
            data = Math.ceil(data/60); // Int minutes
            data += " minutes!";
            paragraph.append(data); // To be replaced by API call. Append whatever the api returns
        }
        else {
            paragraph.append("Unknown");
        }
    })
    .catch(err => {
        console.log(err);
        paragraph.append("Unknown");
    });
  })