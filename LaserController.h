//// LaserController.h
#ifndef LASERCONTROLLER_H
#define LASERCONTROLLER_H

//#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "LaserState.h"
#include <FastLED.h>

#define LED_PIN    16

// Information about the LED strip itself
#define NUM_LEDS    10
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB


#define BRIGHTNESS  64
#define TEMPERATURE OvercastSky

#define FPS             20

// Laser controller declaration
class LaserController
{
//  friend class UI;
  public:
    // Define a laser strip and pass it to lasercontrol to control it easily. 
    LaserController(laserState *lasers, Adafruit_PWMServoDriver* p1, Adafruit_PWMServoDriver* p2, CRGB *l, uint8_t numLeds);
    LaserController(laserState *lasers, Adafruit_PWMServoDriver* p1, Adafruit_PWMServoDriver* p2);

    void init();
    
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
//    uint8_t getMode(void);
    void render();                    // Render values of lasers
    void updateLasers();              // update lasers with changes, 
    void newLastUpdate();
    void toggleLEDS();
    
  private:
    // variables to be initialised
    laserState *m_lasers;    // array with lasers
    int m_nLasers;
    unsigned long lastUpdate;
    Adafruit_PWMServoDriver* pwm1; //on bus 0x40
    Adafruit_PWMServoDriver* pwm2; //on bux 0x41
    bool ledsOn; 
    
    CRGB *leds;
    uint8_t nLeds;

    // render functions
    void defaultFunction();
    void fadeInOut();
    void randomFlash();
    void fadeTester();
    void basicFade();
    void lineSpin();
    void levels();

    // other render
    
    bool newPattern;   // used to initialise pattern, could remove if have classes overiding virtual functions
                              // as initialiser would do the job there
    
    //render function array and control
    static const uint8_t numPatterns = 7; 
    uint8_t currentPattern; 
    unsigned long counter;
    bool toggle; // used for patterns

    // Global laser controls
    uint8_t brightnessScale;       // scale from 0-100
    uint8_t patternSpeed;          // from 0-200???? default is 100
};

#endif /* LEDCONTROLLER_H */
