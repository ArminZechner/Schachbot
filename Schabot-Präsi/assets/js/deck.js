/* ============================================================
   SCHABOT deck: reveal init, speaker indicator, cost chart
   ============================================================ */

Reveal.initialize({
  width: 1280,
  height: 720,
  margin: 0.045,
  minScale: 0.2,
  maxScale: 2.0,

  hash: true,
  history: true,
  controls: true,
  progress: true,
  center: true,
  slideNumber: 'c/t',
  overview: true,
  transition: 'slide',          // default; some slides override via data-transition
  transitionSpeed: 'default',
  backgroundTransition: 'fade',

  highlight: { highlightOnLoad: true },

  plugins: [ RevealNotes, RevealHighlight, RevealZoom, RevealSearch ]
});

/* ---------- speaker indicator ---------- */
(function () {
  var box   = document.getElementById('speaker');
  var elN   = document.getElementById('sp-name');
  var elTo  = document.getElementById('sp-topic');

  function update() {
    var s = Reveal.getCurrentSlide();
    if (!s) return;
    var sp    = s.getAttribute('data-sp')    || 'daniel';
    var name  = s.getAttribute('data-name')  || 'Daniel';
    var topic = s.getAttribute('data-topic') || '';
    box.className = 'speaker sp-' + sp;
    elN.textContent  = name;
    elTo.textContent = topic;
  }
  Reveal.on('ready', update);
  Reveal.on('slidechanged', update);
})();

/* ---------- cost chart (lazy: build when its slide is reached) ---------- */
(function () {
  var built = false;

  var DATA = {                                   // sums to the €413.70 headline
    labels: [
      'Gantry & mechanics',          // 124.67
      'Electronics',                 // motors & drivers 58.74 + microcontroller 15.80
                                     //  + display & input 57.32 + power supply 26.18
                                     //  + electromagnet & magnets 21.05  = 179.09
      'Casing, 3D-print & wiring',   // 3D-print & wiring 63.09 + casing 20.00
      'Shipping & misc'              // 26.85
    ],
    values: [124.67, 179.09, 83.09, 26.85],
    colors: ['#e8b04b', '#5db0e8', '#c8732f', '#8a6b4a']
  };

  function build() {
    if (built) return;
    var canvas = document.getElementById('costChart');
    if (!canvas || typeof Chart === 'undefined') return;
    built = true;

    new Chart(canvas.getContext('2d'), {
      type: 'doughnut',
      data: {
        labels: DATA.labels,
        datasets: [{
          data: DATA.values,
          backgroundColor: DATA.colors,
          borderColor: '#14110d',
          borderWidth: 3,
          hoverOffset: 10
        }]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        animation: { animateRotate: true, duration: 900 },
        cutout: '54%',
        plugins: {
          legend: {
            position: 'right',
            labels: {
              color: '#f3ead7',
              font: { size: 13, family: 'Segoe UI, system-ui, sans-serif' },
              boxWidth: 14, padding: 9
            }
          },
          tooltip: {
            callbacks: {
              label: function (c) { return '  ' + c.label + ': €' + c.parsed.toFixed(2); }
            }
          }
        }
      }
    });
  }

  Reveal.on('ready', function (e) { if (e.currentSlide && e.currentSlide.querySelector('#costChart')) build(); });
  Reveal.on('slidechanged', function (e) { if (e.currentSlide && e.currentSlide.querySelector('#costChart')) build(); });
})();

/* ---------- final demo: play the full video WITH SOUND on entering the slide ----------
   The video has a native controls bar, so the user can drag the progress bar to skip
   to any point (e.g. minute 2). By the time the deck reaches the last slide the user has
   already pressed keys / clicked, so the browser counts that as a user gesture and lets us
   start unmuted. If a browser still blocks it, we retry muted so the video at least plays. */
(function () {
  var video = document.getElementById('demo-video');
  var slide = document.getElementById('demo-slide');
  if (!video || !slide) return;

  function play() {
    video.muted = false;
    var p = video.play();
    if (p && p.catch) {
      p.catch(function () {            // autoplay-with-sound blocked → play muted
        video.muted = true;
        video.play().catch(function () {});
      });
    }
  }

  function onSlide(e) {
    var cur = e ? e.currentSlide : Reveal.getCurrentSlide();
    if (cur === slide) { play(); }     // start from wherever it is; user can scrub freely
    else { video.pause(); }            // leaving: pause but keep position
  }
  Reveal.on('ready', onSlide);
  Reveal.on('slidechanged', onSlide);
})();
