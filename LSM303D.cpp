 #include "LSM303D.h"
#include "Wire.h"
#include <stdint.h>
#include <Arduino.h>

#define LSM_ADDRESS 0b0011101

LSM303D::LSM303D()
{
    i_xOffset = 0;
    i_yOffset = 0;

    f_accPerDigit = 0;
    f_magPerDigit = 0;

    v_maxMag.x = 0;
    v_minMag.x = 0;
    v_maxMag.y = 0;
    v_minMag.y = 0;
    v_maxMag.z = 0;
    v_minMag.z = 0;
}

bool LSM303D::enableDefault()
{
    //Accelerometer

    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
    writeReg(CTRL2, 0x00);
    f_accPerDigit = 0,00006;

    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
    writeReg(CTRL1, 0x57);

    // Magnetometer

    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
    writeReg(CTRL5, 0x64);

    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
    writeReg(CTRL6, 0x20);
    f_magPerDigit = 8/1000000;

    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
    writeReg(CTRL7, 0x00);
    return true;
}

void LSM303D::setOffset(int x0, int y0)
{
    i_xOffset = x0;
    i_yOffset = y0;
}

void LSM303D::writeReg(int8_t reg, int8_t value)
{
    Wire.beginTransmission(LSM_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

int8_t LSM303D::readReg(int8_t reg)
{
    int8_t value;

    Wire.beginTransmission(LSM_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(LSM_ADDRESS,(int8_t) 1);
    value = Wire.read();
    Wire.endTransmission();

    return value;
}

V LSM303D::readRawAcc()
{
    Wire.beginTransmission(LSM_ADDRESS);
    Wire.write(OUT_X_L_A | (1 << 7));
    Wire.endTransmission();
    Wire.requestFrom(LSM_ADDRESS,6);

    int8_t xla = Wire.read();
    int8_t xha = Wire.read();
    int8_t yla = Wire.read();
    int8_t yha = Wire.read();
    int8_t zla = Wire.read();
    int8_t zha = Wire.read();

    Wire.endTransmission();

    v_RawAcc.x = (int16_t)(xha << 8 | xla);
    v_RawAcc.y = (int16_t)(yha << 8 | yla);
    v_RawAcc.z = (int16_t)(zha << 8 | zla);

    return v_RawAcc;
}
/*Problem polega na tym ze Wire.available() <line 118> powinien zwracac ilosc reg dostepnych do odczytu
  a zwraca w obecnym stanie zero 
  -zly adress slave'a? https://www.pololu.com/file/0J703/LSM303D.pdf
  -zly sposob odczytu danych?
  -blad gdzie indziej?
 */
V LSM303D::readRawMag() 
{
    Wire.beginTransmission(LSM_ADDRESS);
    Wire.write(OUT_X_L_M | (1 << 7));
    Wire.endTransmission();
    Wire.requestFrom(LSM_ADDRESS,6);

    Serial.println(LSM_ADDRESS);
    Serial.println(Wire.available());

    int8_t xlm = Wire.read();
    int8_t xhm = Wire.read();
    int8_t ylm = Wire.read();
    int8_t yhm = Wire.read();
    int8_t zlm = Wire.read();
    int8_t zhm = Wire.read();
    Wire.endTransmission();
    
    delay (1000);
    v_RawMag.x = (int16_t)(xhm << 8 | xlm) - i_xOffset;
    v_RawMag.y = (int16_t)(yhm << 8 | ylm) - i_yOffset;
    v_RawMag.z = (int16_t)(zhm << 8 | zlm);

    return v_RawMag;
}

V LSM303D::readNormalizeAcc()
{
    readRawAcc();

    v_NormAcc.x = v_RawAcc.x * f_accPerDigit * 9,80665;
    v_NormAcc.y = v_RawAcc.y * f_accPerDigit * 9,80665;
    v_NormAcc.z = v_RawAcc.z * f_accPerDigit * 9,80665;

    return v_NormAcc;
}

V LSM303D::readScaledAcc()
{
    readRawAcc();

    v_ScalAcc.x = v_RawAcc.x * f_accPerDigit;
    v_ScalAcc.y = v_RawAcc.y * f_accPerDigit;
    v_ScalAcc.z = v_RawAcc.z * f_accPerDigit;

    return v_ScalAcc;
}

V LSM303D::readNormalizeMag()
{
    Serial.print("!");
    readNormalizeMag();

    v_NormMag.x = (v_RawMag.x - i_xOffset) * f_magPerDigit;
    v_NormMag.y = (v_RawMag.y - i_yOffset) * f_magPerDigit;
    v_NormMag.z = v_RawMag.z * f_magPerDigit;

    return v_NormMag;
}

void LSM303D::read()
{
    readNormalizeMag();
    readNormalizeAcc();
}
