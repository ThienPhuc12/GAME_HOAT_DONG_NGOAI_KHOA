/**
 * ESP32 Landmark Builder - Interactive Controller
 * Handles slice transitions, stage updates, auto-build loop, and hook calls.
 */

// State variables
let cur = 'eiffel';
const stgs = { eiffel: 0, dragon: 0 };
let tmr = null;

// Helper selector
const $ = (id) => document.getElementById(id);

/**
 * Set the building stage for a landmark (0 to 5)
 * @param {string} landmark - 'eiffel' or 'dragon'
 * @param {number} stage - 0 to 5
 */
window.setGameStage = (landmark, stage) => {
  landmark = (landmark || 'eiffel').toLowerCase();
  stage = Math.max(0, Math.min(5, +stage || 0));

  if (landmark !== cur) {
    switchLandmark(landmark);
  }

  stgs[landmark] = stage;
  const prefix = landmark === 'eiffel' ? 'eiffel' : 'dragon';

  // Toggle visible slices
  for (let i = 1; i <= 5; i++) {
    const el = $(prefix + '-' + i);
    if (el) {
      el.classList.toggle('built', i <= stage);
    }
  }

  // Update HUD progress
  const pct = Math.round((stage / 5) * 100);
  $('stg').textContent = `STAGE ${stage}/5 (${pct}%)`;
  $('bar').style.width = pct + '%';
  $('cmd').textContent = `setGameStage("${landmark}", ${stage});`;

  // Update jump buttons
  document.querySelectorAll('.jmp').forEach((btn, idx) => {
    btn.classList.toggle('active', idx === stage);
  });

  // Stop auto build once reached the end
  if (stage >= 5 && tmr) {
    stopAuto();
  }
};

/**
 * Switch active landmark tab
 * @param {'eiffel'|'dragon'} name 
 */
function switchLandmark(name) {
  cur = name;
  $('te').classList.toggle('active', name === 'eiffel');
  $('td').classList.toggle('active', name === 'dragon');
  $('svg-eiffel').classList.toggle('active', name === 'eiffel');
  $('svg-dragon').classList.toggle('active', name === 'dragon');
  $('lmk').textContent = name === 'eiffel' ? 'EIFFEL TOWER' : 'DRAGON BRIDGE';

  stopAuto();
  setGameStage(name, stgs[name] || 0);
}

/**
 * Advance to next stage (+1 slice)
 */
function nextBlock() {
  setGameStage(cur, (stgs[cur] + 1) % 6);
}

/**
 * Toggle auto building animation
 */
function toggleAuto() {
  if (tmr) return stopAuto();

  if (stgs[cur] >= 5) {
    setGameStage(cur, 0);
  }

  $('ba').classList.add('btn-p');
  $('ai').textContent = '⏸';

  tmr = setInterval(() => {
    if (stgs[cur] < 5) {
      setGameStage(cur, stgs[cur] + 1);
    } else {
      stopAuto();
    }
  }, 650);
}

/**
 * Stop auto building animation
 */
function stopAuto() {
  if (tmr) {
    clearInterval(tmr);
    tmr = null;
    $('ba').classList.remove('btn-p');
    $('ai').textContent = '▶';
  }
}

// Keyboard shortcut listeners
document.addEventListener('keydown', (e) => {
  if (e.code === 'Space' || e.code === 'ArrowRight') {
    e.preventDefault();
    nextBlock();
  }
  if (e.code === 'KeyR') {
    setGameStage(cur, 0);
  }
  if (e.code === 'KeyA') {
    toggleAuto();
  }
});

// Initialize on page load
document.addEventListener('DOMContentLoaded', () => {
  switchLandmark('eiffel');
});

