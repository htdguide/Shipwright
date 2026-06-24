![Ship of Harkinian](docs/shiptitle.darkmode.png#gh-dark-mode-only)
![Ship of Harkinian](docs/shiptitle.lightmode.png#gh-light-mode-only)

## Website

Official Website: https://www.shipofharkinian.com/

## Discord

Official Discord: https://discord.com/invite/shipofharkinian

If you're having any trouble after reading through this `README`, feel free to ask for help in the Support text channels. Please keep in mind that we do not condone piracy.

# Quick Start

The Ship does not include any copyrighted assets.  You are required to provide a supported copy of the game.

### 1. Verify your ROM dump
You can verify you have dumped a supported copy of the game by using the compatibility checker at https://ship.equipment/. If you'd prefer to manually validate your ROM dump, you can cross-reference its `sha1` hash with the hashes [here](docs/supportedHashes.json).

### 2. Download The Ship of Harkinian from [Releases](https://github.com/HarbourMasters/Shipwright/releases)

### 3. Launch the Game!
#### Windows
* Extract the zip
* Launch `soh.exe`

#### Linux
* Place your supported copy of the game in the same folder as the appimage.
* Execute `soh.appimage`.  You may have to `chmod +x` the appimage via terminal.

#### macOS
* Run `soh.app`. When prompted, select your supported copy of the game.
* You should see a notification saying `Processing OTR`, then, once the process is complete, you should get a notification saying `OTR Successfully Generated`, then the game should start.

#### Nintendo Switch
* Run one of the PC releases to generate an `oot.o2r` and/or `oot-mq.o2r` file. After launching the game on PC, you will be able to find these files in the same directory as `soh.exe` or `soh.appimage`. On macOS, these files can be found in `/Users/<username>/Library/Application Support/com.shipofharkinian.soh/`
* Copy the files to your sd card
```
sdcard
└── switch
    └── soh
        ├── oot-mq.o2r
        ├── oot.o2r
        ├── soh.nro
        └── soh.o2r
```
* Launch via Atmosphere's `Game+R` launcher method.

#### Web (Browser)
* There are no prebuilt web releases. You compile your own bundle from your own ROM — see [Web build (WebAssembly)](#web-build-webassembly) below.
* Once built, serve the `build-emscripten/soh/` folder over HTTP (with the required headers) and open `soh.html` in a WebGL2 browser.

### 4. Play!

Congratulations, you are now sailing with the Ship of Harkinian! Have fun!

# Configuration

### Default keyboard configuration
| N64 | A | B | Z | Start | Analog stick | C buttons | D-Pad |
| - | - | - | - | - | - | - | - |
| Keyboard | X | C | Z | Space | WASD | Arrow keys | TFGH |

### Other shortcuts
| Keys | Action |
| - | - |
| ESC | Toggle menu |
| F2 | Toggle capture mouse input |
| F5 | Save state |
| F6 | Change state |
| F7 | Load state |
| F9 | Toggle Text-to-Speech (Windows and Mac only) |
| F11 | Fullscreen |
| Tab | Toggle Alternate assets |
| Ctrl+R | Reset |

# Project Overview
Ship of Harkinian (SOH) is built atop a custom library dubbed libultraship (LUS). Back in the N64 days, there was an SDK distributed to developers named libultra; LUS is designed to mimic the functionality of libultra on modern hardware. In addition, we are dependant on the source code provided by the OOT decompilation project.

In order for the game to function, you will require a **legally acquired** ROM for Ocarina of Time. Click [here](https://ship.equipment/) to check the compatibility of your specific rom. Any copyrighted assets are extracted from the ROM and reformatted as a .o2r archive file which the code uses.

### Graphics Backends
Currently, there are three rendering APIs supported: DirectX11 (Windows), OpenGL (all platforms), and Metal (MacOS). You can change which API to use in the `Settings` menu of the menubar, which requires a restart.  If you're having an issue with crashing, you can change the API in the `shipofharkinian.json` file by finding the line `gfxbackend:""` and changing the value to `sdl` for OpenGL. DirectX 11 is the default on Windows.

# Web build (WebAssembly)

SoH can be retargeted to emscripten/WebAssembly and run in the browser: the game boots, renders OOT full-canvas via WebGL2, runs game logic, and plays sound through Web Audio.

As with every other platform, **the Ship ships no copyrighted assets — you bring your own ROM.** The web bundle is built *from your own `.o2r` archives*; the copyrighted, ROM-derived data is baked into the WebAssembly bundle at compile time, so a build is personal to you and must not be redistributed.

### 1. Generate your `.o2r` archives first
Run any desktop release (Windows/macOS/Linux) once with your legally acquired ROM, exactly as in [Quick Start](#quick-start). This produces `oot.o2r` (and `soh.o2r`). Place both in the repository root next to `CMakeLists.txt` — the build bundles them into the in-browser filesystem.

> Without `oot.o2r` and `soh.o2r` in the repo root, the web build will not link. This is intentional: no ROM, no build.

### 2. Prerequisites
* [emscripten SDK (emsdk)](https://emscripten.org/docs/getting_started/downloads.html), activated (`source ./emsdk_env.sh`).
* A wasm sysroot containing SDL2, Ogg, Vorbis, Opus and OpusFile built for emscripten. Point CMake at it with `CMAKE_PREFIX_PATH`.
* `cmake` and `ninja`.

### 3. Configure and build
```bash
emcmake cmake -S . -B build-emscripten -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
  -DCMAKE_PREFIX_PATH="/path/to/wasm-sysroot"
cmake --build build-emscripten --target soh
```
Output lands in `build-emscripten/soh/`: `soh.html`, `soh.js`, `soh.wasm` and `soh.data` (your bundled archives).

### 4. Master Quest (optional)
The web build also supports the Master Quest dungeons. **You bring your own MQ ROM, same convention as above:** generate an `oot-mq.o2r` from a supported Master Quest copy and drop it in the repository root *before configuring*. CMake detects it and bundles it into the browser filesystem (you'll see `Web build: bundling oot-mq.o2r (Master Quest) into MEMFS` at configure time); SoH auto-detects it at boot. If the file is absent the build is vanilla-only — no error.

### 5. Serve and play
The build uses pthreads, which require a `SharedArrayBuffer`. Browsers only expose that on a [cross-origin-isolated](https://web.dev/articles/coop-coep) page, so the server **must** send:
```
Cross-Origin-Opener-Policy: same-origin
Cross-Origin-Embedder-Policy: require-corp
```
Serve `build-emscripten/soh/` over HTTP with those headers and open `soh.html` in a WebGL2-capable browser. Opening the file directly via `file://` will not work. The Web Audio context starts suspended until the first click/keypress (browser autoplay policy); sound resumes on that first gesture.

# Custom Assets

Custom assets are packed in `.otr` archive files. To use custom assets, place them in the `mods` folder.

If you're interested in creating and/or packing your own custom asset `.otr` files, check out the following tools:
* [**retro - OTR generator**](https://github.com/HarbourMasters64/retro)
* [**fast64 - Blender plugin**](https://github.com/HarbourMasters/fast64)

# Development
### Building

If you want to manually compile SoH, please consult the [building instructions](docs/BUILDING.md).

### Playtesting
If you want to playtest a continuous integration build, you can find them at the links below. Keep in mind that these are for playtesting only, and you will likely encounter bugs and possibly crashes. 

* [Windows](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-windows.zip)
* [macOS](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-mac.zip)
* [Linux](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-linux.zip)

### Further Reading
More detailed documentation can be found in the 'docs' directory, including the aforementioned [building instructions](docs/BUILDING.md).

* [Credits](docs/CREDITS.md)
* [Custom Music](docs/CUSTOM_MUSIC.md)
* [Formatting](docs/FORMATTING.md)
* [Controller Mapping](docs/GAME_CONTROLLER_DB.md)
* [Modding](docs/MODDING.md)
* [Versioning](docs/VERSIONING.md)

<a href="https://github.com/Kenix3/libultraship/">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./docs/poweredbylus.darkmode.png">
    <img alt="Powered by libultraship" src="./docs/poweredbylus.lightmode.png">
  </picture>
</a>
