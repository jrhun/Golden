#ifndef UI_H
#define UI_H

#include "LaserController.h"

#define MAX_BUTTONS       3   // NEED TO CHANGE FLAGS FROM UINT8_T to UINT16_t to handle more
#define MAX_OUTPUT_LEDS   3

// enum forward declarations (used for UI class)
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
      
    void incButton(buttonPress_t b);
      // Increments either pattern, brightness scale or speed depending on UIState
      
    void decButton(buttonPress_t b);
      // Decrements either pattern, brightness scale or speed depending on UIState

    void (UI::*buttonFunctions[MAX_BUTTONS])(buttonPress_t b) = {&toggleButton, &incButton, &decButton};
//    static const void (*buttonFunctions[])(buttonPress_t b);
      // pointer to each of the button functions (each take a buttonPress variable (shortPress/longPress))
    
    // Pin definitions
    const uint8_t outputPins[MAX_OUTPUT_LEDS] = {A1,A2,A3};             // output pins for feedback of UIState 
    const uint8_t inputPins[MAX_BUTTONS] = {18,14,15};    // these all support pin change interrupt on leonardo
    
    // Button related declarations
    unsigned long lastDebounceTime[MAX_BUTTONS];  //used to debounce buttons. last time pin output toggled
    uint8_t lastPinValue[MAX_BUTTONS];      // last button state
    uint8_t buttonState[MAX_BUTTONS];       // current button state
    
    const unsigned short debounceDelay = 20;
    const unsigned short holdTime[MAX_BUTTONS] = {4000,700,700};
    const unsigned short repeatRate[MAX_BUTTONS] = {4000,200,200}; //repeat rate different for different buttons
    // TODO, repeatRate increases if you hold down for longer
    bool trigger[MAX_BUTTONS] = {true, true, true};
    unsigned long lastTriggered[MAX_BUTTONS] = {0,0,0};
//    unsigned long firstHeld = 0;
//    unsigned long millisHeld[MAX_BUTTONS];
//    unsigned long prevMillisHeld[MAX_BUTTONS] = {0,0,0};
};

#endif // UI_H
