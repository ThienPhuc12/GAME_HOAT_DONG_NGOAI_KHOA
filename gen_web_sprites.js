// Refresh the inline 24x36 Canvas sprites in web/player.html from the
// original 64x96 authoring models in cau-thu-pixel-esp32.html.
// Usage: node gen_web_sprites.js
const fs = require('fs');
const path = require('path');
const vm = require('vm');
const root = __dirname;
const sourceHtml = fs.readFileSync(path.join(root, 'cau-thu-pixel-esp32.html'), 'utf8');
const sourceCode = sourceHtml.split('<script>')[1]?.split('// ---- UI / preview scene.')[0];
if (!sourceCode) throw new Error('PIXEL_PLAYERS source not found');
const scope = {};
scope.globalThis = scope;
vm.runInNewContext(sourceCode, scope, {timeout: 30000});
const source = scope.PIXEL_PLAYERS;
const ids = ['vn', 'cong_phuong', 'fr', 'zidane', 'van_lam', 'lloris'];
const data = {};
for (const id of ids) {
  const role = source.models[id].role;
  const palette = source.getPalette(id);
  const symbols = source.paletteSymbols;
  const lookup = new Map([...symbols].map((symbol, i) => [symbol, i]));
  const frames = [];
  for (let frame = 0; frame < 12; frame++) {
    const rows = source.getFrame(id, role === 'keeper' ? 'front' : 'back', frame, role === 'keeper' ? 'save' : 'shot');
    const bytes = Buffer.alloc(540);
    let acc = 0, bits = 0, pos = 0;
    for (let y = 0; y < 36; y++) for (let x = 0; x < 24; x++) {
      const index = lookup.get(rows[Math.floor(y * 96 / 36)][Math.floor(x * 64 / 24)]);
      if (index === undefined) throw new Error(`Bad palette index in ${id}:${frame}`);
      acc = (acc << 5) | index;
      bits += 5;
      if (bits >= 8) { bits -= 8; bytes[pos++] = (acc >> bits) & 255; acc &= (1 << bits) - 1; }
    }
    if (pos !== 540) throw new Error(`Bad packed size in ${id}:${frame}`);
    frames.push(bytes.toString('base64'));
  }
  data[id] = {pal: [...symbols].map(symbol => palette[symbol] || '#000000'), frames};
}
const webPath = path.join(root, 'web', 'player.html');
const html = fs.readFileSync(webPath, 'utf8');
const begin = '  // PIXEL_ASSETS_START';
const end = '  // PIXEL_ASSETS_END';
const first = html.indexOf(begin), last = html.indexOf(end);
if (first < 0 || last < first) throw new Error('Sprite markers not found');
const replacement = `${begin}\n  const SPRITE_DATA = ${JSON.stringify(data)};\n${end}`;
fs.writeFileSync(webPath, html.slice(0, first) + replacement + html.slice(last + end.length), 'utf8');
console.log(`Embedded ${ids.length * 12} reduced frames in web/player.html`);
