// Order pancake function
(function() {
    document.getElementById("resetSystemStatus").addEventListener("click", resetSystemStatus);
}());

function resetSystemStatus() {
    fetch('http://localhost:5000/set_alarm_level/', {
      method: 'POST'
    })
    .then(() => {
        location.reload();
        //console.log("Succes: ",data);
    })
    .catch(err => {
        console.error(err);
    });
} 