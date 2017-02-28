#include "LaserController.h"

// Static variable definitions
//Laser
static uint8_t laser::nLasers;
static uint8_t laser::nextPin = 0;
static uint8_t laser::currentBus = 0; // 0 for micro PWM, 1 for i2c bus 0x40, 2 for 0x41
static const uint8_t laser::maxLaserOnBus[3] = {4,16,16};
static const uint8_t laser::PRO_PWM_PINS[5] = {3,5,6,9,10};

//LaserController
static const uint8_t numFunctions = 0;
static const void (*renderFunctions[numFunctions])() = {};
static uint8_t brightnessScale = 100;
static uint8_t patternSpeed = 100;


laser::laser(void)
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

uint8_t laser::getNumberLasers()
{
  return nLasers;
}

// ********************************
//  Lasercontroller Implementation
// ********************************

// constructor
LaserController::LaserController(laser *lasers) 
  : lastUpdate(0)
{ 
  // Pass struct for leds
  m_lasers = lasers;
  m_nLasers = lasers->getNumberLasers();
//  lastUpdate = 0;
//  Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);    // default bus is 0x40
//  Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);    // default bus is 0x40
//  
//  // setup pwm I2C output
//  pwm1.begin();
//  pwm1.setPWMFreq(1600);
//  pwm2.begin();
//  pwm2.setPWMFreq(1600);
//  #ifdef TWBR    
//    // save I2C bitrate
//    uint8_t twbrbackup = TWBR;
//    // must be changed after calling Wire.begin() (inside pwm.begin())
//    TWBR = 12; // upgrade to 400KHz!
//  #endif
}

void LaserController::setValue(uint8_t i, uint8_t pvalue, uint16_t fadeTime=0)
{
  if (i >= 0 && i < m_nLasers)
  {
    m_lasers[i].toValue = pvalue;
    m_lasers[i].fadeTime = fadeTime;
//    Serial.print("setValue: \t");
//    Serial.println(m_lasers[i].toValue);
//    Serial.println(m_lasers[i].value);
  }
}

void LaserController::incBrightness(uint8_t amount=5) {
//  brightnessScale = constrain(brightnessScale + amount, 0, 200);
  }
void LaserController::decBrightness(uint8_t amount=5) {
//  brightnessScale = constrain(brightnessScale - amount, 0, 200);
  }
void LaserController::incSpeed(uint8_t amount=5) {
//  patternSpeed = constrain(patternSpeed + amount, 0, 200);
  }
void LaserController::decSpeed(uint8_t amount=5) {
//  patternSpeed = constrain(patternSpeed - amount, 0, 200);
  }
void LaserController::incPattern()
{
//  Serial.print("Inc Pattern");
}
void LaserController::decPattern()
{
//  Serial.print("Dec Pattern");
}
uint8_t LaserController::getValue(uint8_t i)
{
//  Serial.print("getValue:\t");
//  Serial.print(m_lasers[i].value);
//  Serial.println(i);
  if (i >= 0 && i < m_nLasers)
  {

    return m_lasers[i].value;
  } else {return 0;}
}

void LaserController::updateLasers()
{
  unsigned long lastInterval = millis() - lastUpdate;      //essetnailly currentMillis - lastUpdate
  if (lastInterval >= (1000/FPS))
  {
    for (uint8_t i = 0; i < m_nLasers; i++)
    {
      laser *p = &m_lasers[i];  // pointer to current Laser
//      Serial.print("NEXT LOOP!!! m_lasers[");
//      Serial.print(i);
//      Serial.print("]\n\ttoValue:");
//      Serial.print(p->toValue);
//      Serial.print("\n\tvalue:\t");
//      Serial.println(p->value);
      if (p->value == p->toValue)
      {
        continue;
      }
      
//      Serial.print("\tNot equal for i=");
//      Serial.println(i);
      
      if (p->fadeTime == 0) {
         // if no fade time, just set new value
        p->value = p->toValue;
//        Serial.print("\tValue: ");
//        Serial.print(p->value);
        // No need to do the fade calculations 
      } else
      {
        //else calculate fades
        uint8_t steps = p->fadeTime / FPS;  // e.g. 200msec / 20 = 10
        uint8_t diff = abs(p->toValue - p->value);  //e.g.  + 65
        if (p->toValue > p->value)  {
          p->value += diff / steps;   //e.g. 65/10 = 6 . value is now 6, to value 65
        }                             // next time around, 180/20 = 9, diff = 59, 
        else if (p->toValue < p->value)  {
          p->value -= diff / steps;
        }
        // If we are going to go below 1000 / FPS (minimum time to check next)
        // Then set fade time to 0. (Check wrap around)
        if (p->fadeTime - lastInterval < (1000 / FPS))   
          {p->fadeTime = 0;} 
        else  
          {p->fadeTime -= lastInterval;}
      }

      // Set output for laser
      uint8_t output = float (0.0039216) * p->value * p->value;
//      Serial.print("output");
//      Serial.println(output);
      switch(p->i2cBus)
      {
        case 0:
          // Set PWM for pin on pro micro 0-255 (8bit)
          analogWrite(p->pin, output);
          break;
//        case 1:
//          // The code here scales 0-255 evenly to 0-4095
//          pwm1.setPWM(p->pin, 0, (p->value << 4 | p->value >> 4));
//          break;
//        case 2:
//          pwm2.setPWM(p->pin, 0, (p->value << 4 | p->value >> 4));
//          break;
      }
    }
    // update at the end? is that okay?
    lastUpdate = millis(); 
  }
}

void LaserController::render()
{
  // called every 1000/FPS seconds, default is 50msec
  
  
  uint8_t setTo = getValue(0) + 10;
  setTo %= 255; // wrap around at max
  setValue(0, setTo);
  setValue(1, 50);
  setValue(2, 200);
//  Serial.print("Render\n. setTo value:\t");
//  Serial.println(setTo); 
}

