#include <PinChangeInterrupt.h>

#define MAX_BUTTONS   4   // NEED TO CHANGE FLAGS FROM UINT8_T to UINT16_t to handle more
// Generalised class for handling user interface with PCINT library

volatile uint8_t FLAGS = 0;    
                  // 0x1 is Toggle Switch, 
                  // 0x2 is Inc button
                  // 0x4 is dec button
                  // 0x8 is UI detected  **NOT USED**
uint8_t inputPins[3] = {14,15,16};    // these all support pin change interrupt on leonardo

void toggle_button () {FLAGS |= 1 << 0;}
void inc_button ()    {FLAGS |= 1 << 1;}
void dec_button ()    {FLAGS |= 1 << 2;}

void (*ISRFunctions[])() = {toggle_button, inc_button, dec_button};

void setupUI()
{
  for (uint8_t i = 0; i < sizeof(inputPins) / sizeof(inputPins[0]); i++)
  {
    pinMode(inputPins[i], INPUT_PULLUP);
//    pciSetup(i);
    attachPinChangeInterrupt(digitalPinToPCINT(inputPins[i]), ISRFunctions[i], FALLING);
  }
  
//  attachPinChangeInterrupt(digitalPinToPCINT(15), inc_button, RISING);
//  attachPinChangeInterrupt(digitalPinToPCINT(16), dec_button, RISING);
  
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
    if ((FLAGS >> 0) & 1)
    {
      // Toggle Button
      Serial.print("Toggle button Pressed");
      FLAGS &= !((FLAGS >> 0) & 1);
    }
    if ((FLAGS >> 1) & 1)
    {
      // Inc button
      Serial.print("Inc button pressed");
      FLAGS &= !((FLAGS >> 1) & 1);
    }
    if ((FLAGS >> 2) & 1)
    {
      // Dec Button
      Serial.println("Dec button pressed");
      FLAGS &= !((FLAGS >> 2) & 1);
    }
  }
  SREG = oldSREG;     // turn interrupts back on if they were on before
}




















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
