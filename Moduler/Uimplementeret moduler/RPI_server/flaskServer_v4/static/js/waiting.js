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
let timeLeft;
let timeStart = timeEstimate();


async function timeEstimate(){

  fetch('http://localhost:5000/time_estimate/')
  .then((response) => {
      return response.json();
  })
  .then((data) => {
    timeStart = data;
    timeLeft = timeStart;
  }).then(() => {
    bar.animate(1);
    setInterval(function(){
        timeLeft -= 1;
        htmlTime[0].innerHTML = timeLeft;
        console.log(timeLeft/timeStart);
        bar.animate(timeLeft/timeStart);
        if(timeleft == 0){
          return;
        }
    }, 1000);
  })
  .catch(err => {
      console.log(err);
  });
}