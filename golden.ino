#define NUM_LASERS 4

#include "LaserController.h"
#include "UI.h"
// NUM_LASERS defined in LaserController.h


laser lasers[NUM_LASERS];

LaserController controller(lasers);

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
