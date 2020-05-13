// progressbar.js@1.0.0 version is used
// Docs: http://progressbarjs.readthedocs.org/en/1.0.0/

async function startCountdown(){
  var bar = new ProgressBar.Circle(waiting, {
    color: '#FFEA82',
    trailColor: '#eee',
    trailWidth: 1,
    duration: 1000,
    strokeWidth: 2,
    text: {
      value: " ",
      className: 'progressBarTime',
      style: {
        color: '#fff',
        position: 'absolute',
        left: '50%',
        top: '50%',
        padding: '0px',
        margin: '0px',
        transform: 'translate(-50%, -50%)'
      }
    },
    fill: 'rgba(246,118,117,1)',
    from: {color: '#302E53', a:0},
    to: {color: '#302E53', a:1},
    // Set default step function for all animate calls
    step: function(state, circle) {
      circle.path.setAttribute('stroke', state.color);
    }
  }); // Init the countdown bar

  let htmlTime = document.getElementsByClassName('progressBarTime'); // Returns an array of all elements with class progressBarTime 
  let timeLeftInSeconds;
  let timeStart;
  let minutes;
  let seconds;

  let url = document.URL + 'time_estimate/';
  fetch(url)
  .then((response) => {
      return response.json();
  })
  .then((data) => {
    timeStart = data;
    timeStart = 10; // Debugging
    timeLeftInSeconds = timeStart;
    if(timeStart <= 0) {
      togglePancakeButton(1);
      return;
    }
  }).then(() => {
    bar.animate(1);
    let interval = setInterval(function(){
        timeLeftInSeconds -= 1;
        minutes = Math.floor(timeLeftInSeconds/60).toString();
        seconds = (timeLeftInSeconds - minutes * 60).toString();
        if(minutes.length == 1){
          minutes = "0" + minutes;
        }
        if(seconds.length == 1){
          seconds = "0" + seconds;
        }
        time = minutes + ":" + seconds;
        htmlTime[0].innerHTML = time;
        bar.animate(timeLeftInSeconds/timeStart);
        if(timeLeftInSeconds < 0){
          htmlTime[0].innerHTML = "00:00";
          clearInterval(interval);
          bar.destroy();
          togglePancakeButton(4);
        }
    }, 1000);
  })
  .catch(err => {
      console.log(err);
  });
}