#include <Wire.h>
#include <BMP180.h>

BMP180 barometer( BMP180_MODEL_ULTRA_HIGH_RES );

void setup()
{
  Serial.begin( 9600 );
  barometer.begin();  
}

void loop()
{
   Serial.print("Temperature = ");
   Serial.println( barometer.getTemperature() );
   
   Serial.print("Pressure = ");
   Serial.println( barometer.getPressure() );
   
   delay( 1000 );
}
