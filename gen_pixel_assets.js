// Extract the authored 5-bit pixel frames and palettes from the HTML source.
// Usage: node gen_pixel_assets.js
const fs = require('fs');
const path = require('path');
const vm = require('vm');

const root = __dirname;
const html = fs.readFileSync(path.join(root, 'cau-thu-pixel-esp32.html'), 'utf8');
const script = html.split('<script>')[1]?.split('// ---- UI / preview scene.')[0];
if (!script) throw new Error('Cannot find PIXEL_PLAYERS source script');
const context = {};
context.globalThis = context;
vm.runInNewContext(script, context, {timeout: 30000});
const source = context.PIXEL_PLAYERS;
if (!source || source.width !== 64 || source.height !== 96 || source.frameCount !== 12)
  throw new Error('Unexpected source sprite format');
const ids = source.playerIds;
if (ids.length !== 8) throw new Error('Expected eight models');
const out = [
  '#pragma once', '#include <stdint.h>', '#include <pgmspace.h>',
  '// Generated from cau-thu-pixel-esp32.html by gen_pixel_assets.js.',
  'static constexpr uint8_t PLAYER_MODEL_COUNT = 8;',
  'static constexpr uint8_t PLAYER_FRAME_COUNT = 12;',
  'static constexpr uint8_t PLAYER_WIDTH = 64;',
  'static constexpr uint8_t PLAYER_HEIGHT = 96;',
  'static constexpr uint16_t PLAYER_FRAME_BYTES = 3840;',
  'static constexpr uint16_t SHOT_CONTACT_MS = 645;',
  'static constexpr uint16_t KEEPER_CONTACT_MS = 610;',
];
const emitArray = (type, name, values, columns = 16) => {
  out.push(`static const ${type} ${name}[${values.length}] PROGMEM = {`);
  for (let i = 0; i < values.length; i += columns) {
    out.push('  ' + values.slice(i, i + columns).map(v => type === 'uint8_t' ?
      '0x' + v.toString(16).padStart(2, '0') : String(v)).join(', ') + ',');
  }
  out.push('};');
};

emitArray('uint16_t', 'SHOT_FRAME_MS', source.frameDurationsMs);
emitArray('uint16_t', 'SAVE_FRAME_MS', source.saveFrameDurationsMs);
for (let model = 0; model < ids.length; model++) {
  const id = ids[model];
  const role = source.models[id].role;
  const palette = source.getRGB565Palette(id);
  if (palette.length !== source.paletteSymbols.length || palette.length > 32)
    throw new Error(`Unexpected palette for ${id}`);
  emitArray('uint16_t', `palette_${model}`, palette);
  for (let frame = 0; frame < 12; frame++) {
    const bytes = Array.from(source.getPackedFrame(id, role === 'keeper' ? 'front' : 'back', frame, role === 'keeper' ? 'save' : 'shot'));
    if (bytes.length !== 3840) throw new Error(`Unexpected frame size for ${id}:${frame}`);
    emitArray('uint8_t', `frame_${model}_${frame}`, bytes, 24);
  }
}
out.push('static const uint8_t* const PLAYER_FRAMES[8][12] PROGMEM = {');
for (let model = 0; model < ids.length; model++)
  out.push('  {' + Array.from({length: 12}, (_, frame) => `frame_${model}_${frame}`).join(', ') + '},');
out.push('};');
out.push('static const uint16_t* const PLAYER_PALETTES[8] PROGMEM = {' + ids.map((_, i) => `palette_${i}`).join(', ') + '};');
out.push('static const uint8_t PLAYER_IS_KEEPER[8] PROGMEM = {' + ids.map(id => source.models[id].role === 'keeper' ? '1' : '0').join(', ') + '};');
out.push('static const uint8_t PLAYER_TEAM[8] PROGMEM = {' + ids.map(id => source.models[id].team === 'fr' ? '0' : '1').join(', ') + '};');
out.push('// Model indices: ' + ids.map((id, i) => `${i}=${id}`).join(', '));
fs.writeFileSync(path.join(root, 'pixel_assets.h'), out.join('\n') + '\n', 'utf8');
console.log(`Generated ${ids.length * 12} frames (${ids.length * 12 * 3840} packed bytes)`);
