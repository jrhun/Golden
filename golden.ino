#define NUM_LASERS 4    // must define before including LaserController (which includes laser state)

#include "LaserController.h"
#include "UI.h"

// laser controller
laserState lasers[NUM_LASERS];
LaserController controller(lasers); 

// UI controller
UI ui(&controller);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  ui.setupUI();
  Serial.println("Setup Complete");
  
}

void loop() {
  // Laser control
  controller.render();       // calculate new values
  controller.updateLasers(); // change laser values
  ui.handleUI();
//  delay(100);
}
