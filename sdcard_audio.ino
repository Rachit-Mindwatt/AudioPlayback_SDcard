#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

// SD Card Pins
#define SD_MISO 13
#define SD_MOSI 11
#define SD_SCK  12
#define SD_CS   10

// I2S Pins for MAX98357A
#define I2S_BCLK 16
#define I2S_LRC  15
#define I2S_DOUT 17

SPIClass spi = SPIClass(FSPI);
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);
  delay(500);

  // Initialize SD Card
  spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spi)) {
    Serial.println("SD Card init failed!");
    while (true);
  }
  Serial.println("SD Card initialized");

  // Setup I2S Output
  out = new AudioOutputI2S();
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  out->SetGain(0.8); // Optional volume control

  // Open the MP3 file
  file = new AudioFileSourceSD("/Trump_WEF_2018.mp3");
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
}

void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      Serial.println("Finished playing MP3.");
    }
  } else {
    delay(1000);
  }
}
