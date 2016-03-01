/* -*- mode: c -*- */
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include "DHT.h"

#define DHTPIN 12     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
static int pos = 0;
static char buffer[80];
int relays[RELAYS]={3,4,5,6,7,8,9,10};

/******************************************
 ** Serial section
 ******************************************/
int readline(int readch, char *buffer, int len)
{
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Newline convention.
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
	if( readch >= ' ' )	/* is Printable ? */
	  {
	    if (pos < len-1) {
	      buffer[pos++] = readch;
	      buffer[pos] = 0;
	    }
	  }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

void prout_int(char *tag, int value)
{
  Serial.print(tag);
  Serial.println(value);
  Serial.flush();
}

void prout_float(char *tag, float value)
{
  Serial.print(tag);
  if ( isnan(value) ) {
    value = 0.0;
  }
  Serial.println(value);
  Serial.flush();
}

void setup() {
  int pin;
  
  for(pin=0; pin<RELAYS; pin++) {
    pinMode(pin, OUTPUT);
  }

  dht.begin();
  Serial.begin(9600);
}

void measure()
{
  prout_int( "C0", analogRead(0) );
  prout_int( "C1", analogRead(1) );
  prout_int( "C2", analogRead(2) );
  prout_float( "H", dht.readHumidity() );
  prout_float( "T", dht.readTemperature() );

}

void relay(int num, int state)
{
  delay(1000);			/* Make certain we don't switch all relays */
  digitalWrite(num, HIGH);
}

void decode(char *buf) {
  while( *buf ) {
    if ( *buf >= 'a' && *buf <= 'h' ) {
      relay( *buf - 'a', LOW);	/* Relay off */
    }
    else if ( *buf >= 'A' && *buf <= 'H' ) {
      relay( *buf - 'H', HIGH);	/* Relay on */
    }
    else if ( *buf == '*' ) {
      measure();
    }
    else if ( *buf == '@' ) {
      Serial.println("v0.1");
    }
    else if ( *buf == '?' ) {
      Serial.println("a..h, realy off");
      Serial.println("A..H, realy on");
      Serial.println("*     return measurements");
      Serial.println("*       T for temp, H for humidity, C for CO2");
    }
    buf++;
  }
}

void loop() {
  if (readline(Serial.read(), buffer, 80) > 0) {
    decode(buffer);
  }
}
