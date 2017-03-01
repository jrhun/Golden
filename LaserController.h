//// LaserController.h
#ifndef LASERCONTROLLER_H
#define LASERCONTROLLER_H

//#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "LaserState.h"

#define FPS             20

// Laser controller declaration
class LaserController
{
//  friend class UI;
  public:
    // Define a laser strip and pass it to lasercontrol to control it easily. 
    LaserController(laserState *lasers);
    
    void setValue(uint8_t i, uint8_t pvalue, uint16_t fadeTime=0);
      //Set a laser value and optionally fade time
    uint8_t getValue(uint8_t i);
      // Return laser[i].value
    uint8_t getToValue(uint8_t i);
      // Return laser[i].toValue
    uint16_t getFadeTime(uint8_t i);
      //
      
    // UI Functions
    void incBrightness(uint8_t);
    void decBrightness(uint8_t);
    void incSpeed(uint8_t);
    void decSpeed(uint8_t);
    void incPattern();
    void decPattern();

    // Render Function
    void setPattern( uint8_t i);     // set currentFunction 
//    void getPattern( uint8_t i);
    void render();                    // Render values of lasers
    void updateLasers();              // update lasers with changes, 
    
    
  private:
    // variables to be initialised
    laserState *m_lasers;    // array with lasers
    int m_nLasers;
    unsigned long lastUpdate;
//    Adafruit_PWMServoDriver pwm1; //on bus 0x40
//    Adafruit_PWMServoDriver pwm2; //on bux 0x41

    // render functions
    void defaultFunction();
    void fadeInOut();
    void randomFlash();
    void fadeTester();
    void basicFade();
    
    bool newPattern;   // used to initialise pattern, could remove if have classes overiding virtual functions
                              // as initialiser would do the job there
    
    //render function array and control
    static const uint8_t numPatterns = 5; 
    uint8_t currentPattern; 
    unsigned long counter;
    bool toggle; // used for patterns

    // Global laser controls
    uint8_t brightnessScale;       // scale from 0-100
    uint8_t patternSpeed;          // from 0-200???? default is 100
};

#endif /* LEDCONTROLLER_H */
