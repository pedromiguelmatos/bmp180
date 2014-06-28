#include "Arduino.h"
#include "Wire.h"
#include "BMP180.h"

BMP180::BMP180( int mode )
{
	OSS = mode;		
}

bool BMP180::begin()
{
	Wire.begin();
	
	ac1 = readInt16(0xAA);
	ac2 = readInt16(0xAC);
	ac3 = readInt16(0xAE);
	ac4 = readInt16(0xB0);
	ac5 = readInt16(0xB2);
	ac6 = readInt16(0xB4);
	b1 = readInt16(0xB6);
	b2 = readInt16(0xB8);
	mb = readInt16(0xBA);
	mc = readInt16(0xBC);
	md = readInt16(0xBE);
}
float BMP180::getTemperature(){
	long x1, x2, b5;
	
	unsigned long ut = get_ut();
	x1 = ( ( ut - (long) ac6 ) * (long) ac5 ) >> 15;
	x2 = ( (long) mc << 11 ) /( x1 + md );
	b5 = x1 + x2;

	float temp = ((b5 + 8)>>4);
	temp = temp / 10;

	return temp;
}

long BMP180::getPressure(){
	long x1, x2, x3, b3, b5, b6, p;
	unsigned long b4, b7;
	
	unsigned long up = get_up();
	unsigned int ut = get_ut();
	
	x1 = ( ( ut - (long) ac6 ) * (long) ac5 ) >> 15;
	x2 = ( (long) mc << 11 ) /( x1 + md );
	b5 = x1 + x2;
	
	b6 = b5 - 4000;
	x1 = ( b2 * ( b6 * b6 ) >> 12 )>> 11;
	x2 = ( ac2 * b6 ) >> 11;
	x3 = x1 + x2;
	b3 = ( ( (  ac1 * 4 + x3 ) << OSS ) + 2 ) >> 2;

	x1 = ( ac3 * b6 ) >> 13;
	x2 = ( b1 * ( ( b6 * b6 ) >> 12 ) ) >> 16;
	x3 = ( ( x1 + x2 ) + 2) >> 2;
	b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
	b4 = (ac4 * (x3 + 32768 ) ) >> 15;

	b7 = ( (unsigned long) up - b3 ) * ( 50000 >> OSS );
	
	if ( b7 < 0x80000000 ) {
		p = ( b7 << 1 ) / b4;
		} else {
		p = ( b7 / b4 ) << 1;
	}
	
	x1 = ( p >> 8 ) * ( p >> 8 );
	x1 = ( x1 * 3038 ) >> 16;
	x2 = ( -7357 * p ) >> 16;
	p += ( x1 + x2 + 3791 ) >> 4;

	return p;
}

char BMP180::readByte( char addr )
{
	Wire.beginTransmission( 0x77 );
	Wire.write( addr );
	Wire.endTransmission();
	
	Wire.requestFrom( 0x77 , 1 );
	while ( !Wire.available()) {}

	return Wire.read();
}

unsigned int BMP180::get_ut()
{
	Wire.beginTransmission( 0x77 );
	Wire.write( 0xF4 );
	Wire.write( 0x2E );
	Wire.endTransmission();
	
	delay( 5 );
	
	return readInt16( 0xF6 );
}

unsigned long BMP180::get_up(){
	
	Wire.beginTransmission( 0x77 );
	Wire.write( 0xF4 );
	Wire.write( 0x34 + ( OSS << 6 ) );
	Wire.endTransmission();
	
	delay( 2 + ( 3 << OSS ) );
	
	return readInt24( 0xF6 ) >> ( 8 - OSS );
}

int BMP180::readInt16( char addr )
{
	Wire.beginTransmission( 0x77 );
	Wire.write( addr );
	Wire.endTransmission();
	
	Wire.requestFrom( 0x77 , 2 );
	while ( Wire.available() < 2 ) {}

	return ( Wire.read() << 8 ) | Wire.read();
}

unsigned long BMP180::readInt24( char addr )
{
	Wire.beginTransmission( 0x77 );
	Wire.write( addr );
	Wire.endTransmission();
	
	Wire.requestFrom( 0x77 , 3 );
	while ( Wire.available() < 3 ) {}

	return ( (unsigned long) Wire.read() << 16 ) | ( (unsigned long) Wire.read() << 8) | (unsigned long) Wire.read();
}