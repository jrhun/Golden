#ifndef LASERSTATE_H
#define LASERSTATE_H

#include <stdint.h>

// Class for holding laser groups
//const uint8_t max_pins[] = {3,16,16};   // max number of pins for each output method
class laserState
{
  public:
    uint8_t pin;        // output pin (PWM enabled or i2c bus)
    uint8_t i2cBus;     // 0 = micro PWM pin, 1 = i2c bus 0x40, 2 = i2c bus 0x41 
    uint8_t value;      // pwm value 0-255
    uint16_t fadeTime;  // Amount of time LEFT in fade in msec
    uint8_t toValue;    // space to hold value to fade to
  
    laserState (void);
//    laserState (uint8_t pin, uint8_t currentBus);
    uint8_t getNumberLasers();
 
  private:
    static uint8_t nLasers;
//    static uint8_t nextPin;
//    static uint8_t currentBus; // 0 for micro PWM, 1 for i2c bus 0x40, 2 for 0x41
//    static const uint8_t maxLaserOnBus[3];
//    static const uint8_t PRO_PWM_PINS[4];
};



#endif // LASERSTATE_H
