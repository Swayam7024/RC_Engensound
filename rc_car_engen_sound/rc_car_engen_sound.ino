

// Include the necessary libraries

#include <Servo.h>
#include <TMRpcm.h>

// Define the pin numbers
#define THROTTLE_PIN A0
#define SPEAKER_PIN 9

// Create servo object to read throttle input
Servo throttle;

// Create TMRpcm object to play audio
TMRpcm audio;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Attach throttle input to pin
  throttle.attach(THROTTLE_PIN);

  // Initialize audio playback
  audio.speakerPin = SPEAKER_PIN;
  audio.quality(0); // Set audio quality (0-1, 1 is highest quality)

  // Start playing engine sound
  audio.loop("car engen sound.mp3"); // Replace "engine.wav" with your engine sound file name
}

void loop() {
  // Read throttle position
  int throttlePosition = throttle.read();

  // Map throttle position to engine sound parameters
  int volume = map(throttlePosition, 0, 1023, 0, 30); // Adjust volume based on throttle position
  //int pitch = map(throttlePosition, 0, 1023, 500, 1000); // Adjust pitch based on throttle position

  // Set volume and pitch for engine sound
  audio.setVolume(volume);
  //audio.setPitch(pitch);

  // Print throttle position (for debugging)
  Serial.println(throttlePosition);

  // Delay to prevent overwhelming the serial output and to allow for smooth audio playback
  delay(500);
}
