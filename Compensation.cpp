#include "Compensation.h"
#include "LSM303D.h"
#include "math.h"
float Compensation::noTiltCompensation(V mag)
{
    heading = atan2(mag.y, mag.x);
    return heading;
}

float Compensation::tiltCompensation(V acc, V mag)
{
    roll = asin(acc.y);
    pitch = asin(-acc.x);
    
    cosRoll = cos(roll);
    sinRoll = sin(roll);
    cosPitch = cos(pitch);
    sinPitch = sin(pitch);
    
    Xh = mag.x * cosPitch + mag.z * sinPitch;
    Yh = mag.x * sinRoll * sinPitch + mag.y * cosRoll - mag.z * sinRoll * cosPitch;
    
    heading = atan2(Yh, Xh);
    
    return heading;
}

float Compensation::declinationAngle(float heading, float degrees, float minutes, bool positivity)
{
    heading += (degrees + (minutes / 60.0)) / (180/M_PI);
    return heading;
}

float Compensation::fixAngle(float heading)
{
    if (heading < 0)
        heading += 2 * M_PI;
    if (heading > 2 * M_PI)
        heading -= 2 * M_PI;
        
    return heading;
}

float Compensation::convToDegrees(float heading)
{
    heading = heading * 180 / M_PI;
    return heading;
}
