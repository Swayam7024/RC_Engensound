/*
 * ------------------------------------------------------------------
 *  RC Car Engine Sound - engine sound volume follows the throttle
 * ------------------------------------------------------------------
 *  Board  : Arduino Uno / Nano (ATmega328P)
 *  Wiring : A0  -> RC receiver throttle channel (signal)
 *           D9  -> speaker amplifier input (TMRpcm audio out)
 *           SD card module: CS -> D4, MOSI -> D11, MISO -> D12, SCK -> D13
 *           GND -> common ground (receiver, SD module, amplifier)
 *  Sound  : "engine.wav" in the SD card root, 8-bit unsigned PCM,
 *           mono, 16-32 kHz sample rate
 *  Author : Swayam Khantwal (github.com/Swayam7024)
 *  License: MIT
 * ------------------------------------------------------------------
 */

// Include the necessary libraries
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>

// Define the pin numbers
#define THROTTLE_PIN A0      // receiver throttle signal (used as a digital input)
#define SPEAKER_PIN 9        // TMRpcm audio output (Timer1)
#define SD_CS_PIN 4          // SD card module chip select

// Sound file on the SD card (8-bit mono WAV)
const char ENGINE_SOUND_FILE[] = "engine.wav";

// RC throttle pulse range (µs)
const unsigned long THROTTLE_MIN_US = 1000;
const unsigned long THROTTLE_MAX_US = 2000;
const unsigned long PULSE_TIMEOUT_US = 25000;  // no pulse within 25 ms -> no signal

// TMRpcm volume range (0-7)
const int VOLUME_IDLE = 2;   // engine idle volume
const int VOLUME_MAX  = 7;   // full throttle volume

// Create TMRpcm object to play audio
TMRpcm audio;

int currentVolume = -1;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  pinMode(THROTTLE_PIN, INPUT);

  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card init failed - check wiring and card.");
    while (true) {}
  }

  // Initialize audio playback
  audio.speakerPin = SPEAKER_PIN;
  audio.quality(0);   // 0 = normal, 1 = 2x oversampling (needs more CPU)
  audio.setVolume(VOLUME_IDLE);

  // Start playing the engine sound and keep looping it
  audio.loop(1);
  audio.play(ENGINE_SOUND_FILE);
}

void loop() {
  // Read the throttle pulse from the receiver.
  // pulseInLong() uses micros(), so it stays accurate while TMRpcm's
  // audio interrupts are running (plain pulseIn() would read too short).
  unsigned long throttlePulse = pulseInLong(THROTTLE_PIN, HIGH, PULSE_TIMEOUT_US);

  int volume;
  if (throttlePulse == 0) {
    volume = VOLUME_IDLE;   // no receiver signal -> idle sound
  } else {
    throttlePulse = constrain(throttlePulse, THROTTLE_MIN_US, THROTTLE_MAX_US);
    // Map throttle position to engine sound volume
    volume = map(throttlePulse, THROTTLE_MIN_US, THROTTLE_MAX_US, VOLUME_IDLE, VOLUME_MAX);
  }

  // Only change the volume when it actually changes
  if (volume != currentVolume) {
    audio.setVolume(volume);
    currentVolume = volume;
  }

  // Restart the sound if playback stopped for any reason
  if (!audio.isPlaying()) {
    audio.play(ENGINE_SOUND_FILE);
  }

  // Print throttle pulse and volume (for debugging)
  Serial.print(throttlePulse);
  Serial.print(" us -> volume ");
  Serial.println(volume);

  // Short delay to keep the serial output readable
  delay(50);
}
