#include <stdint.h>
#ifndef LSM303D_H_
#define LSM303D_H_



#ifndef V_STRUCT_H
  #define V_STRUCT_H 
  struct V
  {
    float x;
    float y;
    float z;
  };
  #endif
  
class LSM303D
{
public:
  enum register_adress           //https://www.pololu.com/file/0J703/LSM303D.pdf
  {
    //right justified data of temperature sensors
    TEMP_OUT_L = 0x05,
    TEMP_OUT_H = 0x06,

    //magnetometer status register
    STATUS_M = 0x07,

    //magnetometer output data 16-bits
    OUT_X_L_M = 0x08,
    OUT_X_H_M = 0x09,
    OUT_Y_L_M = 0x0A,
    OUT_Y_H_M = 0x0B,
    OUT_Z_L_M = 0x0C,
    OUT_Z_H_M = 0x0D,

    //device identification register 01001001
    WHO_AM_I = 0x0F,

    //interrupts
    INT_CTRL_M = 0x12,
    INT_SCR_M = 0x13,
    INT_THS_L_M = 0x14,
    INT_THS_H_M = 0x15,

    //offset default = 0
    OFFSET_X_L_M = 0x16,
    OFFSET_X_H_M = 0x17,
    OFFSET_Y_L_M = 0x18,
    OFFSET_Y_H_M = 0x19,
    OFFSET_Z_L_M = 0x1A,
    OFFSET_Z_H_M = 0x1B,

    //reference value for high-pass filter for acceleration data
    REFERENCE_X = 0x1C,
    REFERENCE_Y = 0x1D,
    REFERENCE_Z = 0x1E,

    //control registers
    CTRL0 = 0x1F,
    CTRL1 = 0x20,
    CTRL2 = 0x21,
    CTRL3 = 0x22,
    CTRL4 = 0x23,
    CTRL5 = 0x24,
    CTRL6 = 0x25,
    CTRL7 = 0x26,

    //accelerometer status register
    STATUS_A = 0x27,

    //output accelerometer
    OUT_X_L_A = 0x28,
    OUT_X_H_A = 0x29,
    OUT_Y_L_A = 0x2A,
    OUT_Y_H_A = 0x2B,
    OUT_Z_L_A = 0x2C,
    OUT_Z_H_A = 0x2D,

    //fifo control register & fifo status register
    FIFO_CTRL = 0x2E,
    FIFO_SRC = 0x2F,

    //Inertial interrupt generator
    IG_CFG1 = 0x30,
    IG_SRC1 = 0x31,
    IG_THS1 = 0x32,
    IG_DUR1 = 0x33,
    IG_CFG2 = 0x34,
    IG_SRC2 = 0x35,
    IG_THS2 = 0x36,
    IG_DUR2 = 0x37,

    //?
    CLICK_CFG = 0x38,
    CLICK_SRC = 0x39,
    CLICK_THS = 0x3A,

    //?
    TIME_LIMIT = 0x3B,
    TIME_LATENCY = 0x3C,
    TIME_WINDOW = 0x3D,

    //sleep to wake, return to sleep activation threshold & duration
    Act_THS = 0x3E,
    Act_DUR = 0x3F,
  };
  enum sa0state                  //less significant bit of the device address
  {
    sa0_low,
    sa0_high,
    sa0_auto,
  };

    LSM303D ();

    V readRawAcc();
    V readNormalizeAcc();
    V readScaledAcc();
    V readRawMag();
    V readNormalizeMag();

    void writeReg(int8_t reg, int8_t value);
    int8_t readReg(int8_t reg);
    void read();

    void setOffset(int x0, int y0);
    bool enableDefault ();

    private:

    V v_RawAcc;
    V v_NormAcc;
    V v_ScalAcc;
    V v_RawMag;
    V v_NormMag;
    V v_maxMag;
    V v_minMag;

    float f_accPerDigit;
    float f_magPerDigit;

    int i_xOffset, i_yOffset;

};



#endif /* LSM303D_H_ */