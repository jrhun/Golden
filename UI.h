#ifndef UI_H
#define UI_H

#include <PinChangeInterrupt.h>
#include "LaserController.h"

#define MAX_BUTTONS       3   // NEED TO CHANGE FLAGS FROM UINT8_T to UINT16_t to handle more
#define MAX_OUTPUT_LEDS   3

// enum declarations (used for UI class)
enum UIState_t : uint8_t; 
enum buttonPress_t : uint8_t;   // could remove from header?
enum buttons_t : uint8_t;

// Generalised class for handling user interface with PCINT library
class UI
{
	// take a copy of lasercontroller when constructing as needs to edit values in it
	public:
	  // Pass a reference to laserCOntroller for constructor
    //    We change values in this. 
	  UI(LaserController *c);
   
    void setupUI();
    void handleUI();
    
  private:
    LaserController *controller; 
    UIState_t UIState;

    // Functions for controlling UI state
    void toggleButton(buttonPress_t b);
      // Changes UIState. button press can be shortPress or longPress
      
    void incButton();
      // Increments either pattern, brightness scale or speed depending on UIState
      
    void decButton();
      // Decrements either pattern, brightness scale or speed depending on UIState

    static void (*buttonFunctions[MAX_BUTTONS])(buttonPress_t) = {toggleButton, incButton, decButton};
    
    // Pin definitions
    const uint8_t outputPins[MAX_OUTPUT_LEDS] = {18,19,20};             // output pins for feedback of UIState 
    const uint8_t inputPins[MAX_BUTTONS] = {14,15,16};    // these all support pin change interrupt on leonardo
    
    // Button related declarations
    unsigned long lastDebounceTime[MAX_BUTTONS];  //used to debounce buttons. last time pin output toggled
    
    uint8_t lastPinValue[MAX_BUTTONS]; 
    const unsigned short debounceDelay = 25;
    const unsigned short holdTime = 700;
};

#endif // UI_H
