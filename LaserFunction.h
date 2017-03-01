#ifndef LASERFUNCTION_H
#define LASERFUNCTION_H

#ifndef NUM_LASERS
#define NUM_LASERS      4
#endif

#include "LaserController.h"

class laserFunction
{
  public:
    LaserState *laser;
    laserFunction()
    {
      ;
    }
    void defaultFn()
    {
      uint8_t setTo = getValue(0) + 10;
      setTo %= 255; // wrap around at max
      setValue(0, setTo);
      setValue(1, 50);
      setValue(2, 200);
    }
};





#endif /* LASERFUNCTION_H */
