#include <Servo.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 60

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

Servo myservo;  // create servo object to control a servo
int pos=1; // variable to store the servo position

void setup() {
  nfc.begin();
  nfc.SAMConfig(); // configure board to read RFID tags 
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.clear();         //   Set all pixels in RAM to 0 (off)
}


void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
void loop() {
  if(pos==1){
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object "Digital 3"
    myservo.writeMicroseconds(2300);
    pos=2;
    delay(1000);

  }
  {
  strip.fill((0, 0, 0), 0, 0);
  strip.fill((0, 0, 0), 0);
  strip.show();   // Send the updated pixel colors to the hardware.
  }
  
  if(pos==3){
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object "Digital 3"
    myservo.writeMicroseconds(1000);
    pos=4;
    delay(1000);
  }
  if(pos==2){
    myservo.detach();
  }
  if(pos==4){
    myservo.detach();
    delay(700);
  }
  String ringUid;
  boolean success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};  // Buffer to store the returned UID
  uint8_t uidLength; // Length of the UID (4)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success & uidLength==4) {

    for (uint8_t i=0; i<4; i++){
      ringUid += String(uid[i], HEX);
    }
    if (ringUid=="c3ab6a1c" || ringUid=="2da05538"){
      Serial.println("scanned");
      if(pos!=4){
        pos=3;
      }
      if(pos==3){
        rainbow(0);             // Flowing rainbow cycle along the whole strip  
      }
      if(pos==4){
        pos=1;
        rainbow(0);             // Flowing rainbow cycle along the whole strip  
        rainbowOff;
    }
  }
}
      }
    
  void theaterChaseRainbow(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
void rainbow(int wait) {
  // Hue of first pixel runs 2 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 2*65536. Adding 256 to firstPixelHue each time
  // means we'll make 2*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 1*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
  }
}
void rainbowOff (int wait) {
    strip.fill((0, 0, 0), 0, 0);
  }
 
