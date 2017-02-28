#define NUM_LASERS 4

#include "LaserController.h"
#include "UI.h"

// laser controller
laser lasers[NUM_LASERS];
LaserController controller(lasers); 

// UI controller
UI ui(&controller);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Setup Complete");
  setupUI();
}

void loop() {
  // Laser control
  controller.render();       // calculate new values
  controller.updateLasers(); // change laser values
  handleUI();
//  delay(100);
}
