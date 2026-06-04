#include <emscripten.h>

#include <stdint.h>

extern "C" {

EM_JS(int, monopoly_web_canvas_measure_text_width, (const char *text, int count, int pixelHeight, int weight), {
  try {
    if (!text || count <= 0) return 0;
    var value = UTF8ToString(text, count);
    var canvas = Module.__monopolyTextCanvas;
    if (!canvas) {
      canvas = (typeof OffscreenCanvas !== 'undefined') ? new OffscreenCanvas(1, 1) : document.createElement('canvas');
      Module.__monopolyTextCanvas = canvas;
    }
    canvas.width = 1;
    canvas.height = 1;
    var ctx = canvas.getContext('2d');
    if (!ctx) return 0;
    var px = Math.max(7, Math.min(96, pixelHeight || 16));
    var fontWeight = weight >= 650 ? 700 : 400;
    ctx.font = fontWeight + ' ' + px + 'px Arial, sans-serif';
    return Math.max(1, Math.ceil(ctx.measureText(value).width));
  } catch (e) {
    return 0;
  }
});

EM_JS(int, monopoly_web_canvas_raster_text, (const char *text, int count, int pixelHeight, int weight,
                                             unsigned int colorRef, unsigned char *outRgba,
                                             int outWidth, int outHeight), {
  try {
    if (!text || !outRgba || count <= 0 || outWidth <= 0 || outHeight <= 0) return 0;
    var value = UTF8ToString(text, count);
    var canvas = Module.__monopolyTextCanvas;
    if (!canvas) {
      canvas = (typeof OffscreenCanvas !== 'undefined') ? new OffscreenCanvas(1, 1) : document.createElement('canvas');
      Module.__monopolyTextCanvas = canvas;
    }
    canvas.width = outWidth;
    canvas.height = outHeight;
    var ctx = canvas.getContext('2d');
    if (!ctx) return 0;
    ctx.clearRect(0, 0, outWidth, outHeight);
    var px = Math.max(7, Math.min(96, pixelHeight || 16));
    var fontWeight = weight >= 650 ? 700 : 400;
    var r = colorRef & 255;
    var g = (colorRef >> 8) & 255;
    var b = (colorRef >> 16) & 255;
    ctx.font = fontWeight + ' ' + px + 'px Arial, sans-serif';
    ctx.textBaseline = 'top';
    ctx.fillStyle = 'rgb(' + r + ',' + g + ',' + b + ')';
    ctx.fillText(value, 0, 0);
    var data = ctx.getImageData(0, 0, outWidth, outHeight).data;
    HEAPU8.set(data, outRgba);
    return 1;
  } catch (e) {
    return 0;
  }
});

EM_JS(void, monopoly_web_audio_unlock, (), {
  try {
    var AudioCtor = self.AudioContext || self.webkitAudioContext;
    if (!AudioCtor) return;
    if (!Module.__monopolyAudio) {
      Module.__monopolyAudio = { ctx: new AudioCtor(), nextId: 1, handles: new Map() };
    }
    Module.__monopolyAudio.ctx.resume();
  } catch (e) {
  }
});

EM_JS(void *, monopoly_web_audio_play_wav_data, (const unsigned char *data, unsigned int size,
                                                 int loop, int volume, int pan), {
  try {
    if (!data || !size) return 0;
    var AudioCtor = self.AudioContext || self.webkitAudioContext;
    if (!AudioCtor) return 0;
    if (!Module.__monopolyAudio) {
      Module.__monopolyAudio = { ctx: new AudioCtor(), nextId: 1, handles: new Map() };
    }
    var audio = Module.__monopolyAudio;
    var ctx = audio.ctx;
    ctx.resume();
    var id = audio.nextId++;
    var bytes = HEAPU8.slice(data, data + size);
    function read16(offset) {
      return bytes[offset] | (bytes[offset + 1] << 8);
    }
    function readS16(offset) {
      var value = read16(offset);
      return (value & 0x8000) ? value - 0x10000 : value;
    }
    function read32(offset) {
      return (bytes[offset] | (bytes[offset + 1] << 8) | (bytes[offset + 2] << 16) | (bytes[offset + 3] << 24)) >>> 0;
    }
    function fourcc(offset) {
      return String.fromCharCode(bytes[offset], bytes[offset + 1], bytes[offset + 2], bytes[offset + 3]);
    }
    function clamp16(value) {
      if (value < -32768) return -32768;
      if (value > 32767) return 32767;
      return value;
    }
    function sampleToFloat(value) {
      return Math.max(-1, Math.min(1, value / 32768));
    }
    function parseWave() {
      if (bytes.length < 44 || fourcc(0) !== 'RIFF' || fourcc(8) !== 'WAVE') return null;
      var fmt = null;
      var dataOffset = 0;
      var dataSize = 0;
      var offset = 12;
      while (offset + 8 <= bytes.length) {
        var idText = fourcc(offset);
        var chunkSize = read32(offset + 4);
        var chunkData = offset + 8;
        if (chunkData > bytes.length || chunkSize > bytes.length - chunkData) break;
        if (idText === 'fmt ' && chunkSize >= 16) {
          fmt = {
            format: read16(chunkData),
            channels: read16(chunkData + 2),
            sampleRate: read32(chunkData + 4),
            byteRate: read32(chunkData + 8),
            blockAlign: read16(chunkData + 12),
            bits: read16(chunkData + 14),
            samplesPerBlock: 0,
            coefficients: []
          };
          if (chunkSize >= 20) fmt.samplesPerBlock = read16(chunkData + 18);
          if (fmt.format === 2 && chunkSize >= 22) {
            var coefficientCount = read16(chunkData + 20);
            var coefficientOffset = chunkData + 22;
            for (var coefIndex = 0; coefIndex < coefficientCount &&
                 coefficientOffset + 4 <= chunkData + chunkSize; ++coefIndex) {
              fmt.coefficients.push([
                readS16(coefficientOffset),
                readS16(coefficientOffset + 2)
              ]);
              coefficientOffset += 4;
            }
          }
        } else if (idText === 'data') {
          dataOffset = chunkData;
          dataSize = chunkSize;
        }
        offset = chunkData + chunkSize + (chunkSize & 1);
      }
      if (!fmt || !dataOffset || !dataSize || !fmt.channels || !fmt.sampleRate || !fmt.blockAlign) return null;
      return { fmt: fmt, dataOffset: dataOffset, dataSize: dataSize };
    }
    function createBufferFromChannels(channels, sampleRate) {
      var frames = 0;
      for (var ch = 0; ch < channels.length; ++ch) {
        if (channels[ch].length > frames) frames = channels[ch].length;
      }
      if (frames <= 0) return null;
      var buffer = ctx.createBuffer(channels.length, frames, sampleRate);
      for (var outCh = 0; outCh < channels.length; ++outCh) {
        var dst = buffer.getChannelData(outCh);
        var src = channels[outCh];
        for (var frame = 0; frame < frames; ++frame) {
          dst[frame] = frame < src.length ? src[frame] : 0;
        }
      }
      return buffer;
    }
    function decodePcmWave(wave) {
      var fmt = wave.fmt;
      if (fmt.format !== 1 || (fmt.bits !== 8 && fmt.bits !== 16)) return null;
      var sourceChannels = Math.max(1, fmt.channels);
      var outputChannels = Math.min(2, sourceChannels);
      var bytesPerSample = fmt.bits >> 3;
      var frames = Math.floor(wave.dataSize / fmt.blockAlign);
      if (frames <= 0) return null;
      var buffer = ctx.createBuffer(outputChannels, frames, fmt.sampleRate);
      for (var ch = 0; ch < outputChannels; ++ch) {
        var dst = buffer.getChannelData(ch);
        var sampleOffset = wave.dataOffset + ch * bytesPerSample;
        for (var frame = 0; frame < frames; ++frame) {
          var p = sampleOffset + frame * fmt.blockAlign;
          if (p + bytesPerSample > bytes.length) break;
          if (fmt.bits === 8) {
            dst[frame] = (bytes[p] - 128) / 128;
          } else {
            var value = bytes[p] | (bytes[p + 1] << 8);
            if (value & 0x8000) value -= 0x10000;
            dst[frame] = Math.max(-1, Math.min(1, value / 32768));
          }
        }
      }
      return buffer;
    }
    function decodeImaAdpcmWave(wave) {
      var fmt = wave.fmt;
      if (fmt.format !== 17 || fmt.blockAlign <= fmt.channels * 4) return null;
      var sourceChannels = Math.max(1, fmt.channels);
      var outputChannels = Math.min(2, sourceChannels);
      var samplesPerBlock = fmt.samplesPerBlock || Math.floor(((fmt.blockAlign - sourceChannels * 4) * 2) / sourceChannels + 1);
      var channels = [];
      for (var outCh = 0; outCh < outputChannels; ++outCh) channels.push([]);
      var indexTable = [-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8];
      var stepTable = [
        7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
        34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
        143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449,
        494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411,
        1552, 1707, 1876, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026,
        4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487,
        12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
        32767
      ];
      function decodeNibble(state, nibble) {
        var step = stepTable[state.index];
        var diff = step >> 3;
        if (nibble & 1) diff += step >> 2;
        if (nibble & 2) diff += step >> 1;
        if (nibble & 4) diff += step;
        state.predictor = clamp16((nibble & 8) ? state.predictor - diff : state.predictor + diff);
        state.index += indexTable[nibble & 15];
        if (state.index < 0) state.index = 0;
        if (state.index > 88) state.index = 88;
        return state.predictor;
      }
      var end = Math.min(bytes.length, wave.dataOffset + wave.dataSize);
      var pos = wave.dataOffset;
      while (pos + sourceChannels * 4 <= end) {
        var blockEnd = Math.min(pos + fmt.blockAlign, end);
        var states = [];
        var channelFrames = [];
        for (var ch = 0; ch < sourceChannels; ++ch) {
          var predictor = readS16(pos);
          var index = bytes[pos + 2];
          pos += 4;
          if (index > 88) index = 88;
          states[ch] = { predictor: predictor, index: index };
          channelFrames[ch] = 1;
          if (ch < outputChannels) channels[ch].push(sampleToFloat(predictor));
        }
        while (pos < blockEnd) {
          for (var groupCh = 0; groupCh < sourceChannels && pos < blockEnd; ++groupCh) {
            for (var byteIndex = 0; byteIndex < 4 && pos < blockEnd; ++byteIndex) {
              var packed = bytes[pos++];
              for (var nibbleIndex = 0; nibbleIndex < 2; ++nibbleIndex) {
                if (channelFrames[groupCh] >= samplesPerBlock) continue;
                var nibble = nibbleIndex === 0 ? (packed & 15) : (packed >> 4);
                var sample = decodeNibble(states[groupCh], nibble);
                ++channelFrames[groupCh];
                if (groupCh < outputChannels) channels[groupCh].push(sampleToFloat(sample));
              }
            }
          }
        }
        pos = blockEnd;
      }
      return createBufferFromChannels(channels, fmt.sampleRate);
    }
    function decodeMsAdpcmWave(wave) {
      var fmt = wave.fmt;
      var sourceChannels = Math.max(1, fmt.channels);
      if (fmt.format !== 2 || fmt.blockAlign <= sourceChannels * 7) return null;
      var outputChannels = Math.min(2, sourceChannels);
      var channels = [];
      for (var outCh = 0; outCh < outputChannels; ++outCh) channels.push([]);
      var coefficients = fmt.coefficients.length ? fmt.coefficients : [
        [256, 0], [512, -256], [0, 0], [192, 64],
        [240, 0], [460, -208], [392, -232]
      ];
      var adaptationTable = [230, 230, 230, 230, 307, 409, 512, 614,
                             768, 614, 512, 409, 307, 230, 230, 230];
      var samplesPerBlock = fmt.samplesPerBlock ||
        Math.floor(((fmt.blockAlign - sourceChannels * 7) * 2) / sourceChannels + 2);
      function decodeNibble(ch, nibble, predictor, delta, sample1, sample2) {
        var coef = coefficients[predictor[ch]];
        var signedNibble = nibble >= 8 ? nibble - 16 : nibble;
        var predicted = ((sample1[ch] * coef[0] + sample2[ch] * coef[1]) >> 8) + signedNibble * delta[ch];
        predicted = clamp16(predicted);
        sample2[ch] = sample1[ch];
        sample1[ch] = predicted;
        delta[ch] = (delta[ch] * adaptationTable[nibble & 15]) >> 8;
        if (delta[ch] < 16) delta[ch] = 16;
        return predicted;
      }
      var end = Math.min(bytes.length, wave.dataOffset + wave.dataSize);
      var pos = wave.dataOffset;
      while (pos + sourceChannels * 7 <= end) {
        var blockEnd = Math.min(pos + fmt.blockAlign, end);
        var predictor = [];
        var delta = [];
        var sample1 = [];
        var sample2 = [];
        var channelFrames = [];
        for (var ch = 0; ch < sourceChannels; ++ch) {
          predictor[ch] = bytes[pos++];
          if (predictor[ch] >= coefficients.length) predictor[ch] = 0;
        }
        for (var deltaCh = 0; deltaCh < sourceChannels; ++deltaCh) {
          delta[deltaCh] = readS16(pos);
          pos += 2;
          if (delta[deltaCh] < 16) delta[deltaCh] = 16;
        }
        for (var sample1Ch = 0; sample1Ch < sourceChannels; ++sample1Ch) {
          sample1[sample1Ch] = readS16(pos);
          pos += 2;
        }
        for (var sample2Ch = 0; sample2Ch < sourceChannels; ++sample2Ch) {
          sample2[sample2Ch] = readS16(pos);
          pos += 2;
        }
        for (var seedCh = 0; seedCh < sourceChannels; ++seedCh) {
          channelFrames[seedCh] = 2;
          if (seedCh < outputChannels) {
            channels[seedCh].push(sampleToFloat(sample2[seedCh]));
            channels[seedCh].push(sampleToFloat(sample1[seedCh]));
          }
        }
        var currentByte = 0;
        var highNibble = true;
        function nextNibble() {
          if (highNibble) {
            if (pos >= blockEnd) return -1;
            currentByte = bytes[pos++];
            highNibble = false;
            return currentByte >> 4;
          }
          highNibble = true;
          return currentByte & 15;
        }
        if (sourceChannels === 1) {
          while (channelFrames[0] < samplesPerBlock) {
            var monoNibble = nextNibble();
            if (monoNibble < 0) break;
            var monoSample = decodeNibble(0, monoNibble, predictor, delta, sample1, sample2);
            ++channelFrames[0];
            channels[0].push(sampleToFloat(monoSample));
          }
        } else {
          var done = false;
          while (!done) {
            for (var interleavedCh = 0; interleavedCh < sourceChannels; ++interleavedCh) {
              if (channelFrames[interleavedCh] >= samplesPerBlock) {
                done = true;
                break;
              }
              var nibble = nextNibble();
              if (nibble < 0) {
                done = true;
                break;
              }
              var sample = decodeNibble(interleavedCh, nibble, predictor, delta, sample1, sample2);
              ++channelFrames[interleavedCh];
              if (interleavedCh < outputChannels) channels[interleavedCh].push(sampleToFloat(sample));
            }
          }
        }
        pos = blockEnd;
      }
      return createBufferFromChannels(channels, fmt.sampleRate);
    }
    function decodeGsm610Wave(wave) {
      var fmt = wave.fmt;
      if (fmt.format !== 49 || fmt.channels !== 1 || fmt.blockAlign < 33) return null;

      function saturate(value) {
        if (value < -32768) return -32768;
        if (value > 32767) return 32767;
        return value | 0;
      }
      function gsmAdd(a, b) {
        return saturate(a + b);
      }
      function gsmSub(a, b) {
        return saturate(a - b);
      }
      function gsmMultR(a, b) {
        if (a === -32768 && b === -32768) return 32767;
        return saturate(((a * b) + 16384) >> 15);
      }
      function gsmAsr(a, n) {
        return a >> n;
      }

      var FAC = [18431, 20479, 22527, 24575, 26623, 28671, 30719, 32767];
      var QLB = [3277, 11469, 21299, 32767];
      var MIC = [-32, -32, -16, -16, -8, -8, -4, -4];
      var B = [0, 0, 2048, -2560, 94, -1792, -341, -1144];
      var INVA = [13107, 13107, 13107, 13107, 19223, 17476, 31454, 29708];

      var state = {
        dp0: new Array(280).fill(0),
        v: new Array(9).fill(0),
        LARpp: [new Array(8).fill(0), new Array(8).fill(0)],
        currentLar: 0,
        nrp: 40,
        msr: 0
      };

      function unpackFrame(blockOffset, bitStart, blockEnd) {
        var bitPos = bitStart;
        function getBits(count) {
          var value = 0;
          for (var i = 0; i < count; ++i) {
            var byteOffset = blockOffset + ((bitPos + i) >> 3);
            if (byteOffset >= blockEnd) return -1;
            value |= ((bytes[byteOffset] >> ((bitPos + i) & 7)) & 1) << i;
          }
          bitPos += count;
          return value;
        }

        var frame = {
          LARc: new Array(8),
          Nc: new Array(4),
          bc: new Array(4),
          Mc: new Array(4),
          xmaxc: new Array(4),
          xMc: [new Array(13), new Array(13), new Array(13), new Array(13)]
        };
        var larBits = [6, 6, 5, 5, 4, 4, 3, 3];
        for (var lar = 0; lar < 8; ++lar) {
          frame.LARc[lar] = getBits(larBits[lar]);
          if (frame.LARc[lar] < 0) return null;
        }
        for (var sub = 0; sub < 4; ++sub) {
          frame.Nc[sub] = getBits(7);
          frame.bc[sub] = getBits(2);
          frame.Mc[sub] = getBits(2);
          frame.xmaxc[sub] = getBits(6);
          if (frame.xmaxc[sub] < 0) return null;
          for (var m = 0; m < 13; ++m) {
            frame.xMc[sub][m] = getBits(3);
            if (frame.xMc[sub][m] < 0) return null;
          }
        }
        return frame;
      }

      function decodeLar(LARc) {
        var next = state.currentLar ^ 1;
        for (var i = 0; i < 8; ++i) {
          var temp = gsmAdd(LARc[i], MIC[i]) << 10;
          temp = gsmSub(temp, B[i] << 1);
          temp = gsmMultR(INVA[i], temp);
          state.LARpp[next][i] = gsmAdd(temp, temp);
        }
        var oldLar = state.LARpp[state.currentLar];
        var newLar = state.LARpp[next];
        state.currentLar = next;
        return { oldLar: oldLar, newLar: newLar };
      }

      function xmaxToExpMant(xmaxc) {
        var exp = 0;
        if (xmaxc > 15) exp = (xmaxc >> 3) - 1;
        var mant = xmaxc - (exp << 3);
        if (mant === 0) {
          exp = -4;
          mant = 7;
        } else {
          while (mant <= 7) {
            mant = (mant << 1) | 1;
            --exp;
          }
          mant -= 8;
        }
        return { exp: exp, mant: mant };
      }

      function decodeRpe(Mc, xmaxc, xMc) {
        var expMant = xmaxToExpMant(xmaxc);
        var temp1 = FAC[expMant.mant];
        var temp2 = 6 - expMant.exp;
        var temp3 = 1 << (temp2 - 1);
        var xMp = new Array(13);
        for (var i = 0; i < 13; ++i) {
          var temp = ((xMc[i] << 1) - 7) << 12;
          temp = gsmMultR(temp1, temp);
          temp = gsmAdd(temp, temp3);
          xMp[i] = gsmAsr(temp, temp2);
        }
        var erp = new Array(40).fill(0);
        for (var j = 0; j < 13; ++j) {
          var index = Mc + j * 3;
          if (index < 40) erp[index] = xMp[j];
        }
        return erp;
      }

      function longTermSynthesis(frame) {
        var wt = new Array(160);
        for (var sub = 0; sub < 4; ++sub) {
          var erp = decodeRpe(frame.Mc[sub], frame.xmaxc[sub], frame.xMc[sub]);
          var nr = frame.Nc[sub];
          if (nr < 40 || nr > 120) {
            nr = state.nrp;
          } else {
            state.nrp = nr;
          }
          var brp = QLB[frame.bc[sub]];
          var base = 120 + sub * 40;
          for (var k = 0; k < 40; ++k) {
            var drpp = gsmMultR(brp, state.dp0[base + k - nr]);
            var sample = gsmAdd(erp[k], drpp);
            state.dp0[base + k] = sample;
            wt[sub * 40 + k] = sample;
          }
        }
        return wt;
      }

      function larToRp(LARp) {
        var rp = new Array(8);
        for (var i = 0; i < 8; ++i) {
          var temp = Math.abs(LARp[i]);
          if (temp < 11059) {
            temp <<= 1;
          } else if (temp < 20070) {
            temp += 11059;
          } else {
            temp = (temp >> 2) + 26112;
          }
          rp[i] = LARp[i] < 0 ? -temp : temp;
        }
        return rp;
      }

      function shortTermSegment(wt, sr, start, end, LARp) {
        var rp = larToRp(LARp);
        for (var k = start; k < end; ++k) {
          var sri = wt[k];
          for (var i = 7; i >= 0; --i) {
            sri = gsmSub(sri, gsmMultR(rp[i], state.v[i]));
            state.v[i + 1] = gsmAdd(state.v[i], gsmMultR(rp[i], sri));
          }
          state.v[0] = sri;
          sr[k] = sri;
        }
      }

      function interpolateLar(larPair, mode) {
        var LARp = new Array(8);
        for (var i = 0; i < 8; ++i) {
          var oldValue = larPair.oldLar[i];
          var newValue = larPair.newLar[i];
          if (mode === 0) {
            LARp[i] = gsmAdd(gsmAdd(gsmAsr(oldValue, 2), gsmAsr(newValue, 2)), gsmAsr(oldValue, 1));
          } else if (mode === 1) {
            LARp[i] = gsmAdd(gsmAsr(oldValue, 1), gsmAsr(newValue, 1));
          } else if (mode === 2) {
            LARp[i] = gsmAdd(gsmAdd(gsmAsr(oldValue, 2), gsmAsr(newValue, 2)), gsmAsr(newValue, 1));
          } else {
            LARp[i] = newValue;
          }
        }
        return LARp;
      }

      function shortTermSynthesis(frame, wt) {
        var larPair = decodeLar(frame.LARc);
        var sr = new Array(160);
        shortTermSegment(wt, sr, 0, 13, interpolateLar(larPair, 0));
        shortTermSegment(wt, sr, 13, 27, interpolateLar(larPair, 1));
        shortTermSegment(wt, sr, 27, 40, interpolateLar(larPair, 2));
        shortTermSegment(wt, sr, 40, 160, interpolateLar(larPair, 3));
        return sr;
      }

      function postprocess(sr, out) {
        for (var i = 0; i < 160; ++i) {
          state.msr = gsmAdd(sr[i], gsmMultR(state.msr, 28180));
          var sample = gsmAdd(state.msr, state.msr) & -8;
          out.push(sampleToFloat(sample));
        }
      }

      function decodeFrame(frame, out) {
        var wt = longTermSynthesis(frame);
        var sr = shortTermSynthesis(frame, wt);
        postprocess(sr, out);
        for (var i = 0; i < 120; ++i) state.dp0[i] = state.dp0[i + 160];
      }

      var samples = [];
      var end = Math.min(bytes.length, wave.dataOffset + wave.dataSize);
      for (var pos = wave.dataOffset; pos + fmt.blockAlign <= end; pos += fmt.blockAlign) {
        var blockEnd = pos + fmt.blockAlign;
        var framesPerBlock = fmt.blockAlign >= 65 ? 2 : 1;
        for (var frameIndex = 0; frameIndex < framesPerBlock; ++frameIndex) {
          var frame = unpackFrame(pos, frameIndex * 260, blockEnd);
          if (!frame) break;
          decodeFrame(frame, samples);
        }
      }
      return createBufferFromChannels([samples], fmt.sampleRate);
    }
    function decodeWaveToBuffer() {
      var wave = parseWave();
      if (!wave) return null;
      var buffer = decodePcmWave(wave) || decodeImaAdpcmWave(wave) ||
                   decodeMsAdpcmWave(wave) || decodeGsm610Wave(wave);
      if (!buffer && Module.printErr) {
        var fmt = wave.fmt;
        Module.printErr('WebAudio unsupported WAV format: tag=' + fmt.format +
                        ' channels=' + fmt.channels + ' rate=' + fmt.sampleRate +
                        ' bits=' + fmt.bits + ' blockAlign=' + fmt.blockAlign);
      }
      return buffer;
    }
    function startBuffer(buffer, handle) {
      if (handle.stopped) {
        audio.handles.delete(id);
        return;
      }
      var source = ctx.createBufferSource();
      var gain = ctx.createGain();
      source.buffer = buffer;
      source.loop = !!handle.loop;
      gain.gain.value = handle.volume;
      if (ctx.createStereoPanner) {
        var panner = ctx.createStereoPanner();
        panner.pan.value = handle.pan;
        gain.connect(panner);
        panner.connect(ctx.destination);
        handle.panner = panner;
      } else {
        gain.connect(ctx.destination);
      }
      source.connect(gain);
      handle.source = source;
      handle.gain = gain;
      handle.started = true;
      handle.ended = false;
      handle.startedAt = ctx.currentTime;
      handle.bufferDuration = buffer.duration || 0;
      source.onended = function() {
        handle.ended = true;
        handle.source = null;
        if (handle.stopped) audio.handles.delete(id);
      };
      source.start();
    }
    var handle = {
      source: null,
      gain: null,
      panner: null,
      stopped: false,
      started: false,
      ended: false,
      startedAt: 0,
      bufferDuration: 0,
      loop: !!loop,
      volume: Math.max(0, Math.min(1, (volume <= 0 ? 100 : volume) / 100)),
      pan: Math.max(-1, Math.min(1, pan / 100))
    };
    audio.handles.set(id, handle);
    var decodedBuffer = decodeWaveToBuffer();
    if (decodedBuffer) {
      startBuffer(decodedBuffer, handle);
      return id;
    }
    ctx.decodeAudioData(bytes.buffer.slice(0)).then(function(buffer) {
      startBuffer(buffer, handle);
    }).catch(function(err) {
      audio.handles.delete(id);
      if (Module.printErr) Module.printErr('WebAudio decode failed: ' + (err && err.message ? err.message : err));
    });
    return id;
  } catch (e) {
    if (Module.printErr) Module.printErr('WebAudio play failed: ' + (e && e.message ? e.message : e));
    return 0;
  }
});

EM_JS(unsigned int, monopoly_web_audio_get_position_ticks_handle, (void *handleValue, unsigned int ticksPerSecond), {
  try {
    var audio = Module.__monopolyAudio;
    if (!audio || !handleValue || !ticksPerSecond) return 0;
    var handle = audio.handles.get(handleValue);
    if (!handle || !handle.started) return 0;
    var duration = handle.bufferDuration || 0;
    if (handle.ended && !handle.loop) {
      return duration > 0 ? Math.max(1, Math.round(duration * ticksPerSecond)) : 0;
    }
    var elapsed = Math.max(0, audio.ctx.currentTime - handle.startedAt);
    if (handle.loop && duration > 0) elapsed = elapsed % duration;
    if (!handle.loop && duration > 0 && elapsed > duration) elapsed = duration;
    return Math.max(0, Math.round(elapsed * ticksPerSecond));
  } catch (e) {
    return 0;
  }
});

EM_JS(void, monopoly_web_audio_set_loop_handle, (void *handleValue, int loop), {
  try {
    var audio = Module.__monopolyAudio;
    if (!audio || !handleValue) return;
    var handle = audio.handles.get(handleValue);
    if (!handle) return;
    handle.loop = !!loop;
    if (handle.source) handle.source.loop = handle.loop;
  } catch (e) {
  }
});

EM_JS(void, monopoly_web_audio_stop_handle, (void *handleValue), {
  try {
    var audio = Module.__monopolyAudio;
    if (!audio || !handleValue) return;
    var handle = audio.handles.get(handleValue);
    if (!handle) return;
    handle.stopped = true;
    if (handle.source) {
      try { handle.source.stop(); } catch (e) {}
    }
    audio.handles.delete(handleValue);
  } catch (e) {
  }
});

EM_JS(void, monopoly_web_audio_set_volume_handle, (void *handleValue, int volume), {
  try {
    var audio = Module.__monopolyAudio;
    if (!audio || !handleValue) return;
    var handle = audio.handles.get(handleValue);
    if (!handle) return;
    handle.volume = Math.max(0, Math.min(1, volume / 100));
    if (handle.gain) handle.gain.gain.value = handle.volume;
  } catch (e) {
  }
});

EM_JS(void, monopoly_web_audio_set_pan_handle, (void *handleValue, int pan), {
  try {
    var audio = Module.__monopolyAudio;
    if (!audio || !handleValue) return;
    var handle = audio.handles.get(handleValue);
    if (!handle) return;
    handle.pan = Math.max(-1, Math.min(1, pan / 100));
    if (handle.panner) handle.panner.pan.value = handle.pan;
  } catch (e) {
  }
});

}

static uint16_t read_le16(const unsigned char *p)
{
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}

static uint32_t read_le32(const unsigned char *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

extern "C" unsigned int monopoly_web_audio_wav_duration_ticks(const unsigned char *data,
                                                              unsigned int size,
                                                              unsigned int ticksPerSecond)
{
    if (!data || size < 44 || ticksPerSecond == 0) return 1;
    if (read_le32(data) != 0x46464952u || read_le32(data + 8) != 0x45564157u) return 1;

    uint16_t formatTag = 0;
    uint32_t sampleRate = 0;
    uint32_t byteRate = 0;
    uint16_t blockAlign = 0;
    uint16_t samplesPerBlock = 0;
    uint32_t dataBytes = 0;
    unsigned int offset = 12;
    while (offset + 8 <= size) {
        uint32_t id = read_le32(data + offset);
        uint32_t chunkSize = read_le32(data + offset + 4);
        unsigned int chunkData = offset + 8;
        if (chunkData > size || chunkSize > size - chunkData) break;
        if (id == 0x20746d66u && chunkSize >= 16) {
            formatTag = read_le16(data + chunkData);
            sampleRate = read_le32(data + chunkData + 4);
            byteRate = read_le32(data + chunkData + 8);
            blockAlign = read_le16(data + chunkData + 12);
            if (chunkSize >= 20) samplesPerBlock = read_le16(data + chunkData + 18);
        } else if (id == 0x61746164u) {
            dataBytes = chunkSize;
        }
        offset = chunkData + chunkSize + (chunkSize & 1u);
    }

    if (formatTag == 49 && sampleRate && blockAlign && samplesPerBlock && dataBytes) {
        double blocks = (double)dataBytes / (double)blockAlign;
        double seconds = (blocks * (double)samplesPerBlock) / (double)sampleRate;
        unsigned int ticks = (unsigned int)(seconds * (double)ticksPerSecond + 0.5);
        return ticks ? ticks : 1;
    }

    if (!byteRate || !dataBytes) return 1;
    double seconds = (double)dataBytes / (double)byteRate;
    unsigned int ticks = (unsigned int)(seconds * (double)ticksPerSecond + 0.5);
    return ticks ? ticks : 1;
}
