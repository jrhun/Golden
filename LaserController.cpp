#include "LaserController.h"

// ********************************
//  Lasercontroller Implementation
// ********************************

// constructor
LaserController::LaserController(laserState *lasers, Adafruit_PWMServoDriver* p1, Adafruit_PWMServoDriver* p2, CRGB *l, uint8_t numLeds) 
  : lastUpdate(0), brightnessScale(100), patternSpeed(100), newPattern(true), counter(0), toggle(false)
{ 
  // Pass struct for leds
  m_lasers = lasers;
  m_nLasers = lasers->getNumberLasers();

  pwm1 = p1;
  pwm2 = p2;
}
LaserController::LaserController(laserState *lasers, Adafruit_PWMServoDriver* p1, Adafruit_PWMServoDriver* p2) 
  : lastUpdate(0), brightnessScale(100), patternSpeed(100), newPattern(true), counter(0), toggle(false)
{ 
  // Pass struct for leds
  m_lasers = lasers;
  m_nLasers = lasers->getNumberLasers();

  pwm1 = p1;
  pwm2 = p2;
}

void LaserController::init()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds,NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setTemperature( TEMPERATURE );
//  pwm1.begin();
//  pwm1.setPWMFreq(1600);
//  pwm2.begin();
//  pwm2.setPWMFreq(1600);
  // save I2C bitrate
//  uint8_t twbrbackup = TWBR;
//  // must be changed after calling Wire.begin() (inside pwm.begin())
//  TWBR = 12; // upgrade to 400KHz!
}

void LaserController::setValue(uint8_t i, uint8_t pvalue, uint16_t fadeTime=0)
{
  m_lasers[i].toValue = pvalue;
  m_lasers[i].fadeTime = fadeTime;
}

uint8_t LaserController::getValue(uint8_t i)
{
  return m_lasers[i].value;
}

uint8_t LaserController::getToValue(uint8_t i)
{
  return m_lasers[i].value;
}


void LaserController::incBrightness(uint8_t amount=5) {
  brightnessScale = constrain(brightnessScale + amount, 0, 200);
  Serial.print("Brightness: ");
  Serial.println(brightnessScale);
}
void LaserController::decBrightness(uint8_t amount=5) {
  brightnessScale = constrain(brightnessScale - amount, 0, 200);
  Serial.print("Brightness: ");
  Serial.println(brightnessScale);
}
void LaserController::incSpeed(uint8_t amount=5) {
  patternSpeed = constrain(patternSpeed + amount, 0, 200);
  Serial.print("Pattern Speed: ");
  Serial.println(patternSpeed);
}
void LaserController::decSpeed(uint8_t amount=5) {
  patternSpeed = constrain(patternSpeed - amount, 0, 200);
  Serial.print("Pattern Speed: ");
  Serial.println(patternSpeed);
}
void LaserController::incPattern()
{
  currentPattern++;
  currentPattern %= LaserController::numPatterns;   // wrap around
  newPattern = true;
  Serial.print("Current Pattern: ");
  Serial.println(currentPattern);
}
void LaserController::decPattern()
{
  if (currentPattern == 0)  {currentPattern = LaserController::numPatterns - 1;} 
  else                      {currentPattern--;}
  newPattern = true;
  Serial.print("Current Pattern: ");
  Serial.println(currentPattern);
}


void LaserController::updateLasers()
{
  unsigned long lastInterval = millis() - lastUpdate;      //essetnailly currentMillis - lastUpdate
  if (lastInterval >= (1000/FPS))
  {
    static uint8_t starthue = 0;
    fill_rainbow( leds, nLeds, --starthue, 20);
    for (uint8_t i = 0; i < m_nLasers; i++)
    {
      laserState *p = &m_lasers[i];  // pointer to current Laser
//      if (p->value == p->toValue)
//      {
//        continue;
//      }
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
      if (p->value <= 4)
      {
        p->value = 0;
        p->toValue = 0;
      }

      // Set output for laser
      uint8_t output = float (0.0039216) * p->value * p->value; // map according to quadratic 
      output = constrain(output, 0, brightnessScale);
      switch(p->i2cBus)
      {
        case 0:
          // Set PWM for pin on pro micro 0-255 (8bit)
          analogWrite(p->pin, output);
          break;
        case 1:
          // The code here scales 0-255 evenly to 0-4095
          pwm1->setPin(p->pin, (p->value << 4 | p->value >> 4), false);
          break;
        case 2:
//          Serial.print("OUTPIN PWM2: ");
//          Serial.println(p->value);
          pwm2->setPin(p->pin, (p->value << 4 | p->value >> 4), false);
          break;
      }

      // FastLED code
//      if (i < 10 && p->value > 10)
//      {
//        leds[i].maximizeBrightness();
//        leds[i] %= 192;
//      }
    }
    // update at the end? is that okay?
    lastUpdate = millis();
    FastLED.show(); 
  }
}

void LaserController::setPattern( uint8_t i )
{
  if (i >= 0 && i < LaserController::numPatterns)
  {
    currentPattern = i;
  }
  newPattern = true;
}
//uint8_t LaserController::getMode( uint8_t i )
//{
//  return currentPattern;
//}

void LaserController::defaultFunction()
{
  //Step through each laser, turning them on then off in sequence

  
  static uint8_t currentLaser = 0;
  
  static unsigned long lastUpdate = 0;
  if (newPattern)
  {
    for (int i = 0; i < m_nLasers; i++)
    {
      setValue(i, 0); // turn lasers off if new pattern
    }
    newPattern = false;
  }
  
  if (millis() - lastUpdate > map(patternSpeed,0,200,400,20))
  {
    setValue(currentLaser++, 0);    // turn off current laser
    currentLaser %= m_nLasers;      // wrap around if at 40
    setValue(currentLaser,255);     // turn on next laser
//    Serial.print("Current Laser: ");
//    Serial.print(currentLaser);
//    Serial.print("NumLasers");
//    Serial.println(m_nLasers);
    lastUpdate = millis();
  }  
}
void LaserController::fadeInOut()
{
  for (int i = 0; i < m_nLasers; i++)
  {
    if (newPattern)
    {
      // Initialise
//      m_lasers[i]->fadeTime = 0;
      setValue(i,0,0);
    }
    if (m_lasers[i].fadeTime <= 20)     // if light isn't already changing
    {
      if (getValue(i) == 0)
      {
        setValue(i, 255, map(patternSpeed,0,200,100,800));
      } 
      else
      {
        setValue(i, 0, map(patternSpeed,0,200,100,800));
      }
    }
  }
  if (newPattern) { newPattern = false; }
}
void LaserController::randomFlash()
{

  for (int i = 0; i < m_nLasers; i++)
  {
    //initalise pattern
    if (newPattern) 
    { 
      newPattern = false;
      if (random(0,2) > 0)  // 50/50
      {
        setValue(i,0,0);
      } else 
      {
        setValue(i,random(100,255),0);
      }
    }
    
    if (getValue(i) != 0 && m_lasers[i].fadeTime == 0 && random(0,200) > 100)
    {
      // if value not zero, and NO FADE TIME, set to zero with random fade
      setValue(i,0, random(map(patternSpeed,0,200,500,60)));
    }
    if (getValue(i) == 0 && random(0,200) > map(patternSpeed,0,200,120,40));
    {
      // if value is zero
      setValue(i,random(100,255),0);
    }
  }
}

void LaserController::fadeTester()
{
  if (newPattern)
  {
    newPattern = false;
  }
  for (int i = 0; i < m_nLasers; i++)
  {
    if (m_lasers[i].fadeTime == 0)
    {
      if (newPattern) {setValue(0,255,1000);} 
      else            {setValue(0,0,1000);}
    }
  }
}

void LaserController::basicFade()
{
  uint16_t last = 0;
  for (int i = 0; i < m_nLasers; i++)
  {
    if (newPattern) 
    { 
      setValue(i, last + 100);
      last += 100;
      if (last > 255)
      {
        last = 0;
      }
    }
    uint8_t setTo = getValue(i) + 10;
    setTo %= 255; // wrap around at max
    setValue(i, setTo, 0);
  }
  if (newPattern) { newPattern = false; }
}

void LaserController::lineSpin(){
  static unsigned long lastUpdate = 0;
  if (newPattern)
  {
    for (int i = 0; i < m_nLasers; i++)
    {
      setValue(i, 0); // turn lasers off if new pattern
    }
    newPattern = false;
  }

  static uint8_t currentCol = 0;
  // TODO centralise patternSpeed to be FPS (between 20msec - 300 msec?)
  if (millis() - lastUpdate > map(patternSpeed,0,200,20,300))
  {
    for (uint8_t i = 0; i < m_nLasers; i++)
    {
      if (i % 10 == currentCol)
      { setValue(i,255); } 
      else 
      { setValue(i,0); }
      
    }
    currentCol++
    currentCol %= 10; //only have 10 colums
  }
}

void LaserController::levels()
{
  
}

void LaserController::render()
{
  // called every 1000/FPS seconds, default is 50msec
  // Call the currently selected function
  switch (currentPattern)
  {
    case 0 :
      defaultFunction();
      break;
    case 1 :
      fadeInOut();
      break;
    case 2 :
      randomFlash();
      break;
    case 3 :
      fadeTester(); 
      break;
    case 4 :
      basicFade();
      break;     
    case 5 :
      lineSpin();
    case 6 :
      levels();
  }
//  switch (currentPattern)
//  {
//    case 0;
//      
//  }
}


