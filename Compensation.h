#ifndef COMPENSATION_H
#define COMPENSATION_H

#include "LSM303D.h"

class Compensation
{
    public:
    float noTiltCompensation(V mag);
    float tiltCompensation(V acc, V mag);
    float declinationAngle(float heading, float degrees, float minutes, bool positivity);
    float fixAngle(float heading);
    float convToDegrees(float heading);

    protected:

    private:

    float offX, offY;
    float maxX, minX;
    float maxY, minY;
    float heading;
    float roll;
    float pitch;
    float cosRoll;
    float sinRoll;
    float cosPitch;
    float sinPitch;

    float Xh;
    float Yh;
};

#endif // COMPENSATION_H
