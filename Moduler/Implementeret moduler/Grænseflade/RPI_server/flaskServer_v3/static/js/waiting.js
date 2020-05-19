// progressbar.js@1.0.0 version is used
// Docs: http://progressbarjs.readthedocs.org/en/1.0.0/

var bar = new ProgressBar.Circle(waiting, {
    color: '#FFEA82',
    trailColor: '#eee',
    trailWidth: 1,
    duration: 1000,
    strokeWidth: 6,
    from: {color: '#FFEA82', a:0},
    to: {color: '#ED6A5A', a:1},
    // Set default step function for all animate calls
    step: function(state, circle) {
      circle.path.setAttribute('stroke', state.color);
    }
  });



(function() {
    let state = 1.0
    bar.animate(state)
    setInterval(function(){
        state = state - 0.1
        bar.animate(state)
    }, 1000);
}());