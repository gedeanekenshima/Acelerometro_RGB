#include <Adafruit_NeoPixel.h> //Biblioteca Adafruit Neopixel para WS2812
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "CurieIMU.h" // biblioteca para acelerometro

unsigned long loopTime = 0;          // get the time since program started
unsigned long interruptsTime = 0;    // get the time when motion event is detected

#define PIN            6 // pino digital do WS2812b

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1 //Quantos leds então anexados 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  while(!Serial);   // wait for the serial port to open

  Serial.println("Initializing IMU device...");
  CurieIMU.begin();

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  int axRaw, ayRaw, azRaw,x,y,z;         // raw accelerometer values
  float ax, ay, az;

  // leitura do acelerometro
  CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);

  // convert the raw accelerometer data to G's
  ax = convertRawAcceleration(axRaw);
  ay = convertRawAcceleration(ayRaw);
  az = convertRawAcceleration(azRaw);

  x = map(axRaw,-1023,1023,0,255);
  y = map(ayRaw,-1023,1023,0,255);
  z = map(azRaw,-1023,1023,0,255);
  
  // mostra valores mapeados em x,y e z
  Serial.print("a:\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
  colorWipe(strip.Color(x, y, z), 100); // Red
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;

  return a;
}
// Funcao para mostrar cores 
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
