#define NUM_LASERS 40    // must define before including LaserController (which includes laser state)

#include "LaserController.h"
#include "UI.h"

// laser controller
laserState lasers[NUM_LASERS];
// setup laser state

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
// you can also call it with a different address you want
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

CRGB leds[NUM_LEDS];

LaserController controller(lasers, &pwm1, &pwm2, leds, NUM_LEDS); 


// UI controller
UI ui(&controller);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  
  pwm1.begin();
  pwm1.setPWMFreq(1600);  // This is the maximum PWM frequency
  pwm2.begin();
  pwm2.setPWMFreq(1600);
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
//

  
  controller.init();
  ui.setupUI();
  Serial.println("Setup Complete");
  
}

void loop() {
  // Laser control
  controller.render();       // calculate new values
  controller.updateLasers(); // change laser values
  ui.handleUI(); 
//  delay(100);
}
