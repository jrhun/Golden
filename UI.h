#include <PinChangeInterrupt.h>

#define MAX_BUTTONS   3   // NEED TO CHANGE FLAGS FROM UINT8_T to UINT16_t to handle more
// Generalised class for handling user interface with PCINT library

enum buttonModes = {PATTERN = 0, BRIGHTNESS = 1, SPEED = 2}; 
buttonModes buttonState = PATTERN;

volatile uint8_t FLAGS = 0;    
                  // 0x1 is Toggle Switch falling, 
                  // 0x2 is Inc button falling
                  // 0x4 is dec button falling
                  // 0x8 is next button. etc...
                  // 0x16 is toggle switch rising
                  // 0x32 is inc rising
                  
const uint8_t inputPins[MAX_BUTTONS] = {14,15,16};    // these all support pin change interrupt on leonardo
unsigned long lastDebounceTime[MAX_BUTTONS] = {0,0,0};  //used to debounce buttons. last time pin output toggled
unsigned short debounceDelay = 25;

// Interrupt service routines (attached to pin change interrupt) Separated to rising and falling
void ISR_toggleButtonF () {FLAGS |= 1 << 0;}
void ISR_incButtonF ()    {FLAGS |= 1 << 1;}
void ISR_decButtonF ()    {FLAGS |= 1 << 2;}
void (*ISRFunctionsF[MAX_BUTTONS])() = {ISR_toggleButtonF, ISR_incButtonF, ISR_decButtonF};

//void ISR_toggleButtonR() {FLAGS |= 1 << 4;}
//void ISR_incButtonR ()    {FLAGS |= 1 << 5;}
//void ISR_decButtonR ()    {FLAGS |= 1 << 6;}
//void (*ISRFunctionsR[MAX_BUTTONS])() = {ISR_toggleButtonR, ISR_incButtonR, ISR_decButtonR};

// Code to be called when buttons pressed
void toggleButton()
{
  Serial.print("Toggle button Pressed");
  // Short press Change pattern

  // Long press change mode
  // Possible modes: pattern (default), brightness, speed. Inditacted by lights
}
void incButton()
{
  Serial.print("Inc button Pressed");
  // if mode == PATTERN, then increment the pattern
  switch(buttonState){
    case PATTERN :
      // increment pattern
      break;
    case BRIGHTNESS :
      // inc brightness
      break;
    case SPEED :
      // inc speed
      break;
  }
    
}
void decButton()
{
  Serial.print("Dec button Pressed");
  // if mode == BRIGHTNESS
}
void (*buttonFunction[MAX_BUTTONS])() = {toggleButton, incButton, decButton};   //order matters!

//alternative w. double click support
//void toggleButton(uint8_t clickType)
//{
//  // clickType 0 = normal, 1 = double, 2 = hold. 
//}

void setupUI()
{
  for (uint8_t i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++)
  {
    pinMode(inputPins[i], INPUT_PULLUP);
  // Setup interrupts for each buttons
    attachPinChangeInterrupt(digitalPinToPCINT(inputPins[i]), ISRFunctionsF[i], FALLING);
//    attachPinChangeInterrupt(digitalPinToPCINT(inputPins[i]), ISRFunctionsR[i], RISING);
  }  
}

void handleUI()
{
  // TODO Stop jitter (i.e. don't repress if within 20msec)
  // TODO repress inc and dec after a period of time (700msec, start increasing every 300msec)
  // Maybe use another flag bit?
  byte oldSREG = SREG;    // remember if interupts are on or off
  noInterrupts();         // turn interupts off while accessing shared variable
  
  if (FLAGS > 0)   // check if any flag bits are set
  {
    for (uint8_t i = 0; i < MAX_BUTTONS; i++)
    {
      // check if rising
      if ( ((FLAGS >> i) & 1) and (millis() - lastDebounceTime[i] > debounceDelay) )
      {
        lastDebounceTime[i] = millis();
        buttonFunction[i]();
        
        //clear rising flag
        FLAGS &= !((FLAGS >> i) & 1);
      }
    }
//    if ( ((FLAGS >> 0) & 1) and lastDebounceTime[0])
//    {
//      // Toggle Button
//      Serial.print("Toggle button Pressed");
//      FLAGS &= !((FLAGS >> 0) & 1);
//    }
//    if ((FLAGS >> 1) & 1)
//    {
//      // Inc button
//      Serial.print("Inc button pressed");
//      FLAGS &= !((FLAGS >> 1) & 1);
//    }
//    if ((FLAGS >> 2) & 1)
//    {
//      // Dec Button
//      Serial.println("Dec button pressed");
//      FLAGS &= !((FLAGS >> 2) & 1);
//    }
  }
  SREG = oldSREG;     // turn interrupts back on if they were on before

  
}

//class UI
//{
//  // Implementing multi-click (adapated from salsaman)
//  
//  // Timing values
//  uint8_t debounce = 20;
//  uint16_t DClength = 250;
//  uint16_t holdTime = 1000;
//
//  // Button state
//  boolean buttonVal[MAX_BUTTONS] = HIGH;  // start with pullup
//  boolean buttonLast[MAX_BUTTONS] = HIGH; 
//  boolean DCwaiting[MAX_BUTTONS] = false; //waiting for double click
//  boolean singleOK[MAX_BUTTONS] = true;
//  long downTime[MAX_BUTTONS] = -1;
//  long upTime[MAX_BUTTONS] = -1;
//  
//};
//
//int UI::checkButton() {    
//   int event = 0;
//   buttonVal = digitalRead(buttonPin);
//   // Button pressed down
//   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
//   {
//       downTime = millis();
//       ignoreUp = false;
//       waitForUp = false;
//       singleOK = true;
//       holdEventPast = false;
//       longHoldEventPast = false;
//       if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
//       else  DConUp = false;
//       DCwaiting = false;
//   }
//   // Button released
//   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
//   {        
//       if (not ignoreUp)
//       {
//           upTime = millis();
//           if (DConUp == false) DCwaiting = true;
//           else
//           {
//               event = 2;
//               DConUp = false;
//               DCwaiting = false;
//               singleOK = false;
//           }
//       }
//   }
//   // Test for normal click event: DCgap expired
//   if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
//   {
//       event = 1;
//       DCwaiting = false;
//   }
//   // Test for hold
//   if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
//       // Trigger "normal" hold
//       if (not holdEventPast)
//       {
//           event = 3;
//           waitForUp = true;
//           ignoreUp = true;
//           DConUp = false;
//           DCwaiting = false;
//           //downTime = millis();
//           holdEventPast = true;
//       }
//       // Trigger "long" hold
//       if ((millis() - downTime) >= longHoldTime)
//       {
//           if (not longHoldEventPast)
//           {
//               event = 4;
//               longHoldEventPast = true;
//           }
//       }
//   }
//   buttonLast = buttonVal;
//   return event;
//}

















//class UI
//{
//  // 
//  UI(uint8_t nButtons, uint8_t &aFLAGS)
//  {
//    // allocate space in array for each button
//    numButtons = nButtons;
//    buttons = new uint8_t[numButtons];
//    buttonsAdded = 0; 
//    FLAGS = aFLAGS;
//  }
//
//  // Add each button before initialising 
//  // add a #define TOGGLE_B, or INC_B or DEC_B for each button/pin
//  // so that this can be called as addButton(TOGGLE_B);
//  // button number matches pin
//  void addButton(uint8_t pin)
//  {
//    buttons[buttonsAdded++] = pin;
//  }
//
//  void init()
//  {
//    for (uint8_t i; i < numButtons; i++)
//    {
//      // care, as buttonPins may not be initialised
//      pinMode(buttonPins[i], INPUT_PULLUP);
//    }
//    // setup ISR 
//  }
//
//  bool ButtonPressed()
//  {
//    
//  }
//
//  uint8_t getButtonPressed()
//  {
//    
//  }
//  
//  
//  private:
//    const uint8_t MAX_BUTTONS
//    uint8_t *FLAGS; 
//    
//    uint8_t numButtons;
//    uint8_t buttonsAdded; 
//    uint8_t *buttons[]; // stores pin of each button
//
//    void pciSetup(byte pin); 
//    {
//        *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
//        PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
//        PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
//    }
//};

//ISR (PCINT0_vect)
//{
//  
//}  // end of PCINT0_vect
