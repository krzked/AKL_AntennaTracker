#include "Calibration.h"
#include "LSM303D.h"

float Calibration::findOffsetX(V v_mag)
{
    if(v_mag.x < f_minX)
        f_minX = v_mag.x;
    if(v_mag.x > f_maxX)
        f_maxX = v_mag.x;
        
    f_offX = (f_maxX + f_minX)/2;
    
    return f_offX;
}

float Calibration::findOffsetY(V v_mag)
{
    if(v_mag.y < f_minY)
        f_minY = v_mag.y;
    if(v_mag.y > f_maxY)
        f_maxY = v_mag.y;
        
    f_offY = (f_maxY + f_minY)/2;
    
    return f_offY;
}

void Calibration::resetOffsets()
{
    f_minX = 0;
    f_minY = 0;
    f_maxX = 0;
    f_maxY = 0;
    f_offX = 0;
    f_offY = 0;
}
