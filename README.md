# Monopoly(1999) WebAssembly Port

![Monopoly Menu Screen](https://i.imgur.com/yVAykNP.jpeg)

[Play in your browser](https://mrarm.github.io/Monopoly99_wasm/)

**The web build does not include retail game assets. To play, you must provide your own legally obtained Monopoly CD ISO when the launcher asks for it.**

Retail game assets are not committed, embedded, hosted, or compiled into the WASM output. At runtime, the launcher asks the user to provide a legally obtained ISO which is extracted into the browser's cache. You can find the ISO on many abandonware sites by searching around for it. The correct ISO file will contain a `DAT_MON` folder inside it. Future support for other ISO variations depends on demand.

Regarding the legality of hosting this source code, it was publicly released and still remains available from https://x.com/MrTalida/status/1025016038394613760 in addition to being availiable on GitHub alike from another user's [repo](https://github.com/RetailGameSourceCode/Monopoly). If this is not allowed, please issue a DMCA takedown by following the instructions provided by GitHub [here](https://docs.github.com/en/site-policy/content-removal-policies/dmca-takedown-policy).

## What works

- Gameplay with default settings
- 3d rendering of the board
- Sounds, Voice Dialog, Music
- Custom rules

## What doesn't work

- Firefox appears to have FPS issues running the game. Use something chrome-based for now. Safari runs smooth too.
- Multiplayer (It may be worth a rewrite for modern web standards)
- Saved Games
- Custom boards (Built in boards appear to have some placeholder image with my ISO)
- 2D board mode (Render bug, will fix eventually.)
- Intro video (Not implemented. If you want to watch it: https://www.youtube.com/watch?v=0uwFPGLmydo)
- Some issues exist in the main menu that don't prevent gameplay (compositing issues, buttons sometimes don't respond.)

Expect this to be a bit buggy, this was never meant to run in a web browser on hardware newer than the 2000s. Please submit bug reports!

## Build

Use Emscripten from a shell where `emcmake` and `em++` are available:

```sh
emcmake cmake -S . -B build/web-emscripten -DMONOPOLY_PLATFORM=web
cmake --build build/web-emscripten --target monopoly -j4
```

The web bundle is generated under:

```text
build/web-emscripten/upload/
```

## Run Locally

Serve the generated upload directory with any static file server:

```sh
cd build/web-emscripten/upload
python3 -m http.server 8080
```

Then open:

```text
http://127.0.0.1:8080/
```

The launcher will prompt for the ISO if cached assets are not already available.

The generated `monopoly/Dat_Mon/*.h` ID headers are source metadata and should stay in the repository. The retail `*.dat` files with the same base names must only come from the user's ISO at runtime.

## Source Layout

- `artlib/`: original ArtLib runtime, sequencer, data, rendering, etc.
- `PC3D/`: original PC3D/Direct3D-era rendering code adapted by compatibility shims.
- `monopoly/`: game logic, UI, rules, original project files, and local resource definitions.
- `web/`: Emscripten entry point, launcher, WebGL bridge, browser audio, IndexedDB/ISO asset flow, and compatibility code.
- `ZLib/`: Original zlib library. Could be upgraded.

## Notes for future contributions

- Keep changes at the compatibility boundaries where practical: `web/compat`, `web/monopoly_webgl_bridge.cpp`, `web/monopoly_web_browser.cpp`, and the smallest necessary ArtLib shims.
- Preserve unimplemented systems in source unless a replacement exists. Networking/DirectPlay and voice chat are deferred, not deleted.
- If browser audio regresses, first check the WebAudio bridge and ArtLib sequence-clock behavior.
- If 3D rendering regresses, check DirectDraw/Direct3D compatibility state before changing game logic.

## Use of AI

This project was written using the assistance of an AI coding agent. Most, if not all generated artifacts have been reviewed and rewritten where necessary by a human.

My stance toward the matter is taking a hybrid approach, I do not trust AI enough to let it write anything for me without some level of supervision. AI is a tool, much like a calculator. I wouldn't trust a calculator to be capable of solving every problem, but I know how to utilize it to increase my productivity.
