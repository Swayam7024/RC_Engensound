<div align="center">

# RC Car Engine Sound

**An Arduino sound module that plays an engine sound from an SD card and makes it louder as you open the throttle.**

![Arduino](https://img.shields.io/badge/Arduino-Uno%20%2F%20Nano-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-Arduino-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Audio](https://img.shields.io/badge/Audio-TMRpcm%20WAV-E67E22?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

[Features](#features) · [Hardware](#hardware) · [Getting Started](#getting-started)

</div>

---

## Overview

In this project I synchronise a pre-recorded engine sound with the RC car's throttle stick. The Arduino reads the throttle signal from the RC receiver, plays an engine sound file from an SD card in a continuous loop through a speaker using the `TMRpcm` library, and adjusts the playback volume to the throttle position, so the engine sounds louder as the car accelerates.

## Features

- Engine sound (`engine.wav`) played from an SD card in a continuous loop
- Reads the RC receiver throttle signal (1000–2000 µs) with `pulseInLong()`, which stays accurate while audio interrupts are running
- Volume mapped to throttle: idle level `2` at low throttle up to `7` at full throttle (TMRpcm volume range 0–7)
- Falls back to idle volume if the receiver signal is missing (25 ms timeout)
- Restarts playback automatically if it stops
- PWM audio output on a single pin via `TMRpcm`
- Throttle pulse and volume printed to the Serial Monitor (9600 baud)

## Hardware

| Component | Purpose |
|---|---|
| Arduino board (e.g. Uno / Nano) | Reads the throttle and plays audio |
| SD card module + SD card (FAT16/FAT32) | Stores the engine sound file |
| Speaker with a small amplifier (e.g. PAM8403) or transistor driver | Engine sound output |
| RC receiver (throttle channel) | Throttle input |

## Pin Connections

| Arduino Pin | Connected To |
|---|---|
| A0 | RC receiver throttle channel signal |
| D9 | Speaker amplifier input (audio out) |
| D4 | SD module CS |
| D11 | SD module MOSI |
| D12 | SD module MISO |
| D13 | SD module SCK |
| GND | Common ground (receiver, SD module, amplifier) |

## Sound File Format

`TMRpcm` does **not** play MP3. Prepare the engine sound as:

| Setting | Value |
|---|---|
| File name | `engine.wav` (in the SD card root) |
| Format | WAV, 8-bit unsigned PCM |
| Channels | Mono |
| Sample rate | 16 000 – 32 000 Hz (16 kHz is the safest on an Uno) |

You can convert a sound with [Audacity](https://www.audacityteam.org/) (*Tracks → Mix → Mix Stereo Down to Mono*, set *Project Rate*, then *Export → WAV, Unsigned 8-bit PCM*) or with FFmpeg:

```bash
ffmpeg -i "car engine sound.mp3" -ac 1 -ar 16000 -acodec pcm_u8 engine.wav
```

## How It Works

1. In `setup()`, the SD card is started with `SD.begin(4)`, the speaker pin is set for `TMRpcm`, looping is enabled and `engine.wav` starts playing.
2. In `loop()`, the throttle pulse is measured, constrained to 1000–2000 µs and mapped to a volume of 2–7, which is applied with `audio.setVolume()` whenever it changes.
3. The throttle pulse and volume are printed to the Serial Monitor every loop (about 20 times per second).

## Getting Started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the **TMRpcm** library via **Sketch → Include Library → Manage Libraries**. `SD` and `SPI` are bundled with the IDE.
3. Convert your engine sound to the format above and copy it to the SD card as `engine.wav`.
4. Open `rc_car_engen_sound/rc_car_engen_sound.ino`.
5. Select your board under **Tools → Board** and the correct **Port**, then click **Upload**.
6. Open the Serial Monitor at **9600 baud** to see the throttle readings. If you see `SD card init failed`, check the SD wiring and card format.
7. Adjust `VOLUME_IDLE` and `VOLUME_MAX` in the sketch to taste.

## Project Structure

```
RC_Engensound/
├── rc_car_engen_sound/
│   └── rc_car_engen_sound.ino   # Engine sound sketch
├── LICENSE
└── README.md
```

## Future Improvements

- Vary playback speed / pitch with throttle for a more realistic engine sound
- Add separate idle, rev-up and rev-down sound samples
- Read the throttle with an interrupt instead of `pulseInLong()`

## Author

**Swayam Khantwal** – [GitHub @Swayam7024](https://github.com/Swayam7024)

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
