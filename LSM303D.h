#ifndef LSM303D_H_
#define LSM303D_H_

#include "math.h"
#include <cstddef>
#include "stm32f10x.h"

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
	template <typename T> struct V //vector
	{
		T x, y, z;
	};

	V<int16_t> a;       //acc values
	V<int16_t> m;       //mag values
	V<int16_t> max_m;   //mag max value
	V<int16_t> min_m;   //mag min value

	int8_t last_status;

	LSM303D (void);

	//Init functions//////////////////////////////
	bool init (sa0state = sa0_auto);
	void enableDefault(void);
	//////////////////////////////////////////////

	//Write&Read functions////////////////////////
	void writeAccReg(int8_t reg, int8_t value);
	int8_t readAccReg(int8_t reg);
	void writeMagReg(int8_t reg, int8_t value);
	int8_t reahMagReg(int8_t reg);
	void writeReg(int8_t reg, int8_t value);
	int8_t readReg(int8_t reg);

	void readAcc(void);
	void readMag(void);
	void read(void);
	//////////////////////////////////////////////

	//Timeout handle functions////////////////////
	void setTimeout(unsigned int timeout);
	unsigned int getTimeout(void);
	bool timeoutOccurred(void);
	//////////////////////////////////////////////

	//Heading functions///////////////////////////
	float heading(void);
	template <typename T> float heading(V<T> from);
	//////////////////////////////////////////////

	//V - vector functions////////////////////////
	template <typename Ta, typename Tb, typename To> static void V_cross(const V<Ta> *a, const V<Tb> *b, const V<To> *out);
	template <typename Ta, typename Tb> static float V_dot(const V<Ta> *a, const V<Tb> *b);
	static void V_normalize(V<float> *a);
	/////////////////////////////////////////////


private:
	int8_t acc_adress;
	int8_t mag_adress;

	unsigned int io_timeout;
	bool did_timeout;

	int testReg(int8_t address, register_adress reg);
};

template <typename T> float LSM303D::heading(V<T> from)
{
	V<int32_t> temp_m = {m.x, m.y, m.z};

	temp_m.x -=((int32_t)min_m.x + max_m.x)/2;
	temp_m.y -=((int32_t)min_m.y + max_m.y)/2;
	temp_m.z -=((int32_t)min_m.z + max_m.z)/2;

	V<float> E;
	V<float> N;
	V_cross(&temp_m, &a, &E);
	V_normalize(&E);
	V_cross(&a, &E, &N);
	V_normalize(&N);

	float heading = atan2(vector_dot(&E, &from), vector_dot(&N, &from)) * 180 / 3.14;
	if (heading < 0) heading += 360;
	return heading;
}

template <typename Ta, typename Tb, typename To> void LSM303D::V_cross(const V<Ta> *a, const V<Tb> *b, const V<To> *out)
{
	out->x = (a->y * b->z) - (a->z * b->y);
	out->y = (a->z * b->x) - (a->x * b->z);
	out->z = (a->x * b->y) - (a->y * b->x);
}

template <typename Ta, typename Tb> float LSM303D::V_dot(const V<Ta> *a, const V<Tb> *b)
{
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

#endif /* LSM303D_H_ */