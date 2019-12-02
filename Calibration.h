#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "LSM303D.h"

class Calibration
{
    public:
    
    float findOffsetX(V v_mag);
    float findOffsetY(V v_mag);
    void resetOffsets(void);
    
    private:
        
    float f_minX = 0;
    float f_maxX = 0;
    float f_minY = 0;
    float f_maxY = 0;
    float f_offX = 0;
    float f_offY = 0;
};

#endif // CALIBRATION_H
