#include "LaserState.h"

const uint8_t pinArray[40][2] = { 
  // mapping of each pin to output port
  {0,1}, {2,1}, {4,1}, {8,1}, {10,1}, {15,2}, {13,2}, {11,2}, {7,2}, {5,2},
  {1,1}, {3,1}, {5,1}, {9,1}, {11,1}, {14,2}, {12,2}, {10,2}, {6,2}, {4,2},
  {6,1}, {7,1},{14,1},{12,1}, {13,1}, {9,2}, {8,2},{1,2},{3,2}, {2,2},
  {9,0}, {0,0},{15,1}, {0,0}, { 6,0}, {5,0}, {0,0},{0,2}, {0,0}, {10,0}
};

// Static definitions
static uint8_t laserState::nLasers = 0;
//static uint8_t laserState::nextPin = 0;
//static uint8_t laserState::currentBus = 0; // 0 for micro PWM, 1 for i2c bus 0x40, 2 for 0x41
//static const uint8_t laserState::maxLaserOnBus[3] = {4,16,16};
//static const uint8_t laserState::PRO_PWM_PINS[4] = {5,6,9,10};


laserState::laserState (void)
      : value(0), fadeTime(0), toValue(0)
{
  // Allocate lasers from data table
  pin = pinArray[nLasers][0];
  i2cBus = pinArray[nLasers++][1];
  // automatically allocate new lasers to free pins
//  if (currentBus == 0)
//          { pin = PRO_PWM_PINS[nextPin]; } 
//  else    { pin = nextPin; }
//  
//  i2cBus = currentBus;    // set current bus for this pin
//  nextPin ++;
//  nLasers ++;
//  
//  if (nextPin == maxLaserOnBus[currentBus])
//  {
//    currentBus += 1;     //chagne to next bus if this one is full
//    nextPin = 0;
//  }
};

//laserState::laserState (uint8_t p, uint8_t bus)
//      : value(0), fadeTime(0), toValue(0)
//{
//  // automatically allocate new lasers to free pins
//  
//  pin = p;
//  i2cBus = bus;
//  nLasers ++;
//};

uint8_t laserState::getNumberLasers()
{
  return nLasers;
}

