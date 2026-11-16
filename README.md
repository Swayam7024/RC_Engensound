# RC Car Engine Sound

An Arduino sketch that plays a pre-recorded engine sound and links its volume to the RC car's throttle.

![Platform](https://img.shields.io/badge/Platform-Arduino-00979D?logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?logo=cplusplus&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)
![Status](https://img.shields.io/badge/Status-Work%20in%20progress-orange)

## Overview

In this project I synchronise a pre-recorded engine sound with the RC car's throttle stick. The Arduino plays the engine sound in a loop through a speaker using the `TMRpcm` library and adjusts the playback volume according to the throttle position, so the engine sounds louder as the car accelerates. Pitch control is planned but currently commented out.

## Features

- Continuous looped playback of an engine sound file
- Volume mapped to the throttle position
- PWM audio output on a single pin via `TMRpcm`
- Throttle value printed to the Serial Monitor (9600 baud) for debugging
- Pitch-follows-throttle code prepared (commented out)

## Hardware

| Component | Purpose |
|---|---|
| Arduino board (e.g. Uno / Nano) | Reads the throttle and plays audio |
| Speaker (with a small amplifier or transistor driver) | Engine sound output |
| SD card module + SD card | Stores the sound file read by `TMRpcm` |
| RC receiver (throttle channel) | Throttle input |

## Pin Connections

| Arduino Pin | Connected To |
|---|---|
| A0 | Throttle input (RC receiver throttle channel) |
| D9 | Speaker / amplifier input |
| GND | Common ground |

## How It Works

1. In `setup()`, the speaker pin is set for `TMRpcm` and the engine sound file starts playing in a loop.
2. In `loop()`, the throttle position is read, mapped to a volume level and applied with `audio.setVolume()`.
3. The throttle value is printed to the Serial Monitor every 500 ms.

## Getting Started

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the **TMRpcm** library via **Sketch → Include Library → Manage Libraries**. `Servo` is bundled with the IDE.
3. Copy your engine sound file to the SD card. `TMRpcm` plays 8-bit mono WAV files, so convert the sound to that format and update the file name in the sketch to match.
4. Open `rc_car_engen_sound/rc_car_engen_sound.ino`.
5. Select your board under **Tools → Board** and the correct **Port**, then click **Upload**.
6. Open the Serial Monitor at **9600 baud** to see the throttle readings.

> **Note:** this sketch is a work in progress. The SD card is not initialised in the code yet, the sound file name points to an MP3 (not supported by `TMRpcm`), and the throttle is read with `Servo.read()`, which returns the last value written rather than the receiver signal.

## Project Structure

```
RC_Engensound/
├── rc_car_engen_sound/
│   └── rc_car_engen_sound.ino   # Engine sound sketch
├── LICENSE
└── README.md
```

## Future Improvements

- Initialise the SD card with `SD.begin()` and use a WAV sound file
- Read the receiver throttle signal with `pulseIn()` (or an interrupt) and map 1000–2000 µs to the volume range
- Vary playback speed / pitch with throttle for a more realistic engine sound
- Add separate idle, rev-up and rev-down sound samples

## Author

**Swayam Khantwal** – [GitHub @Swayam7024](https://github.com/Swayam7024)

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
