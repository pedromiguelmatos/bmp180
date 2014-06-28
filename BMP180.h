#ifndef BMP180_h
#define BMP180_h

#include "Arduino.h"
#include "Wire.h"

#define BMP180_MODEL_ULTRA_LOW_POWER	0
#define BMP180_MODEL_STANDART			1
#define BMP180_MODEL_HIGH_RES			2
#define BMP180_MODEL_ULTRA_HIGH_RES		3

class BMP180
{
	public:
		BMP180( int mode );
		bool begin();
		float getTemperature();
		long getPressure();
	private:
		int OSS;
		long ac1, ac2, ac3, b1, b2, mb, mc, md;
		unsigned int ac4, ac5, ac6;
		char readByte( char );
		int readInt16( char addr );
		unsigned long readInt24( char addr );
		unsigned int get_ut();
		unsigned long get_up();
};

#endif