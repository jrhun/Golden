#include "LaserState.h"


// Static definitions
static uint8_t laserState::nLasers;
static uint8_t laserState::nextPin = 0;
static uint8_t laserState::currentBus = 0; // 0 for micro PWM, 1 for i2c bus 0x40, 2 for 0x41
static const uint8_t laserState::maxLaserOnBus[3] = {4,16,16};
static const uint8_t laserState::PRO_PWM_PINS[5] = {3,5,6,9,10};


laserState::laserState (void)
      : value(0), fadeTime(0), toValue(0)
{
  // automatically allocate new lasers to free pins
  if (currentBus == 0)
          { pin = PRO_PWM_PINS[nextPin]; } 
  else    { pin = nextPin; }
  
  i2cBus = currentBus;    // set current bus for this pin
  nextPin ++;
  nLasers ++;
  
  if (nextPin == maxLaserOnBus[currentBus])
  {
    currentBus += 1;     //chagne to next bus if this one is full
    nextPin = 0;
  }
};

uint8_t laserState::getNumberLasers()
{
  return nLasers;
}

