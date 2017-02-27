//// LaserController.h
#ifndef LASERCONTROLLER_H
#define LASERCONTROLLER_H

//#include <stdint.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#ifndef NUM_LASERS
#define NUM_LASERS      4
#endif
#define FPS             20

//#define PRO_PWM_PINS[] = {3,5,6,9,10};   //use DMA output for leds

// where to put below? when making the laser? i.e struct doesn't care about max values
//const uint8_t max_pins[] = {4,16,16};   // max number of pins for each output method

// TODO: automatically fill pro micro, then i2cbus1, then i2cbus2
struct laser
{
  public:
    uint8_t pin;        // output pin (PWM enabled or i2c bus)
    uint8_t i2cBus;     // 0 = micro PWM pin, 1 = i2c bus 0x40, 2 = i2c bus 0x41 
    uint8_t value;      // pwm value 0-255
    uint16_t fadeTime;  // Amount of time LEFT in fade in msec
    uint8_t toValue;    // space to hold value to fade to
  
//    laser(uint8_t ppin=0, uint8_t i2cBus=0, uint8_t value=0);
  laser(void);

  uint8_t getNumberLasers();
  
  private:
    static uint8_t nLasers;
    static uint8_t nextPin;
    static uint8_t currentBus; // 0 for micro PWM, 1 for i2c bus 0x40, 2 for 0x41
    static const uint8_t maxLaserOnBus[3];
    static const uint8_t PRO_PWM_PINS[5];
};

class LaserController
{
  
  public:
    // Define a laser strip and pass it to lasercontrol to control it easily. 
    LaserController(laser *lasers);
  
    //Set a laser value and optionally fade time
    void setValue(uint8_t i, uint8_t pvalue, uint16_t fadeTime=0);

    uint8_t getValue(uint8_t i);
  
    // Render values of lasers
    void render();
    
    // update lasers with changes, 
    void updateLasers();
  private:
    laser *m_lasers;    // array with lasers
    int m_nLasers;
    unsigned long lastUpdate;
//    Adafruit_PWMServoDriver pwm1; //on bus 0x40
//    Adafruit_PWMServoDriver pwm2; //on bux 0x41
};

#endif /* LEDCONTROLLER_H */
