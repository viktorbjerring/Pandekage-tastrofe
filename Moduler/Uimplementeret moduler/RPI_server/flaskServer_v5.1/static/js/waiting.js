// progressbar.js@1.0.0 version is used
// Docs: http://progressbarjs.readthedocs.org/en/1.0.0/

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
  });

let htmlTime = document.getElementsByClassName('progressBarTime'); // Returns an array of all elements with class progressBarTime 
let timeLeftInSeconds;
let timeStart = timeEstimate();
let minutes;
let seconds;

async function timeEstimate(){

  fetch('http://localhost:5000/time_estimate/')
  .then((response) => {
      return response.json();
  })
  .then((data) => {
    timeStart = data;
    timeLeftInSeconds = timeStart;
  }).then(() => {
    bar.animate(1);
    setInterval(function(){
        timeLeftInSeconds -= 1;
        minutes = Math.floor(timeLeftInSeconds/60);
        seconds = timeLeftInSeconds - minutes * 60;
        time = minutes.toString() + ":" + seconds.toString();
        htmlTime[0].innerHTML = time;
        console.log(timeLeftInSeconds/timeStart);
        bar.animate(timeLeftInSeconds/timeStart);
        if(timeLeftInSeconds == 0){
          return
        }
    }, 1000);
  })
  .catch(err => {
      console.log(err);
  });
}