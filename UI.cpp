#include "UI.h"

// Enum definitions
enum UIState_t : uint8_t {pattern, brightness, speed}; 
enum buttonPress_t : uint8_t {shortPress, longPress};
enum buttons_t : uint8_t {toggle_b, inc_b, dec_b};

//static const void (*buttonFunctions[MAX_BUTTONS])(buttonPress_t b) = {UI::toggleButton, UI::incButton, UI::decButton};

// UI Constructor. Default UIState is pattern selection
UI::UI(LaserController *c)
  : UIState(pattern)
{
  controller = c;
  
  unsigned long lastDebounceTime[MAX_BUTTONS] = {0,0,0};  //used to debounce buttons. last time pin output toggled
  unsigned short debounceDelay = 25;

  uint8_t lastPinValue[MAX_BUTTONS] = {HIGH, HIGH, HIGH}; // pills are pulled high
  uint8_t buttonState[MAX_BUTTONS] = {HIGH, HIGH, HIGH};
}

void UI::setupUI()
{
  for (uint8_t i = 0; i < MAX_BUTTONS; i++)
  {
    pinMode(inputPins[i], INPUT_PULLUP);
  }  
  for (uint8_t i = 0; i < MAX_OUTPUT_LEDS; i++)
  {
    pinMode(outputPins[i], OUTPUT);
  }
}

void UI::handleUI()
{
  // TODO Stop jitter (i.e. don't repress if within 20msec)
  // TODO repress inc and dec buttons keep firing after a period of time (700msec, sends button down every 200msec?)

  // for each button
  for (uint8_t i = 0; i < MAX_BUTTONS; i++)
  {
    // Looking for switch that goes low as we have pullup resistors on
    uint8_t reading = digitalRead(inputPins[i]);

    // If the switch changed, due to noise or pressing
    if ( reading != lastPinValue[i] )
    {
      // reset debouncing timer
      lastDebounceTime[i] = millis();
    }

    if ( (millis() - lastDebounceTime[i]) > debounceDelay )
    {
      // Whatever the reading is it, it's been there for longer than debounce delay
      // So take it as current state

      // if button has changed state
      if (reading != buttonState[i]) 
      {
        buttonState[i] = reading; 
        // if button has changed state to LOW reading
        if (reading == LOW)
        { // triger short press
          (this->*buttonFunctions[i])(shortPress);
        }
      }
      // This triggers after a button has been low for > 700msec
      if ( (reading == LOW) and (( millis() - lastDebounceTime[i] ) > holdTime[i]) )
      {
        // If trigger is true, repeat function inc
        if (trigger[i])
        {
          (this->*buttonFunctions[i])(longPress);
          trigger[i] = false;
          lastTriggered[i] = millis();
        }
        // this checks if we have gone past repeatrate
        if ( (millis() - lastTriggered[i]) > repeatRate[i])
        {
          trigger[i] = true; 
        }
      }
    }
    lastPinValue[i] = reading; // save the reading
  }
}

void UI::toggleButton(buttonPress_t p)
{
  Serial.print("Toggle button Pressed");
  // Short press
  if (p == shortPress)
  {
    // change UIState
    switch(UIState) {
      case pattern : 
        UIState = brightness;   break;
      case brightness :
        UIState = speed;        break;
      case speed : 
        UIState = pattern;      break;
    }
    Serial.print("\n\tUI State Changed to: ");
    Serial.println(UIState);
  }
  // Long press ??
  // Possible modes: pattern (default), brightness, speed. Inditacted by lights
  if (p == longPress)
  {
    // do something fun. Maybe default/reset/lasers on&off?
    Serial.println("\tToggle long Press");
  }
}
void UI::incButton(buttonPress_t p)
{
  Serial.print("Inc button Pressed");
  if (p == shortPress)
  {
    Serial.println("\t(Short press)");
    switch(UIState){
    case pattern :
      // increment pattern
      controller->incPattern();
      break;
    case brightness :
      // inc brightness
      controller->incBrightness(5);
      break;
    case speed :
      // inc speed
      controller->incSpeed(5);
      break;
    }
  } else if (p == longPress)
  {
    // long press
    Serial.println("\t(Long press)");
  }
}
void UI::decButton(buttonPress_t p)
{
  Serial.print("Dec button Pressed");
  if (p == shortPress)
  {
    Serial.println("\t(Short press)");
    switch(UIState){
    case pattern :
      // decrement pattern
      controller->decPattern();
      break;
    case brightness :
      // dec brightness
      controller->decBrightness(5);
      break;
    case speed :
      // dec speed
      controller->decSpeed(5);
      break;
    }
  } else if (p == longPress)
  {
    // long press
    Serial.println("\t(Long press)");
  }
}
