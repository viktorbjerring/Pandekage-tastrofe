/* Modified from: https://codepen.io/mranenko/pen/wevamj */

(function() {
	var hamburger = {
		navToggle: document.querySelector('.nav-toggle'),
		nav: document.querySelector('nav'),

		doToggle: function(e) {
			e.preventDefault();
			this.navToggle.classList.toggle('expanded');
			this.nav.classList.toggle('expanded');
		}
	};
	hamburger.navToggle.addEventListener('click', function(e) { hamburger.doToggle(e); });
}());



$('.nav-toggle-bar').click(function(){
	if($("#nav-icon").hasClass("fa-info-circle")) {
		$( "#nav-icon" ).fadeOut(250,function() {
			$("#nav-icon").toggleClass('fa-info-circle fa-times-circle')
			
			$( "#nav-icon" ).delay(1).fadeIn(250, function() {
		});
		});
	} else {
		$( "#nav-icon" ).fadeOut(20,function() {
			$("#nav-icon").toggleClass('fa-info-circle fa-times-circle')
			
			$( "#nav-icon" ).delay(1).fadeIn(20, function() {
		});
		});
	}
});