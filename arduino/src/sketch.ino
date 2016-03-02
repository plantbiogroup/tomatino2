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

#define RELAYS 6
unsigned char relays[RELAYS]={4,5,6,7,8,9};
/* unsigned char relays[RELAYS]={6,7}; */
/* #define RELAYS 4 */
/* unsigned char relays[RELAYS]={6,7,8,9}; */

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

void activate(int pin)
{
  pinMode(relays[pin], OUTPUT);
}


void relay(unsigned char num, unsigned char state)
{
  Serial.print("Setting pin ");
  Serial.print(num);
  Serial.print(" to ");
  Serial.println(state);
  delay(1000);
  digitalWrite(relays[num], state);
}

void decode(char  cr) {
  if ( cr <= ' ' ) {
    return;
  }
  else if ( cr >= 'a' && cr <= 'h' ) {
    relay( cr - 'a', LOW);	/* Relay off */
  }
  else if ( cr >= 'A' && cr <= 'H' ) {
    relay( cr - 'A', HIGH);	/* Relay on */
  }
  else if ( cr >= '0' && cr <= '9' ) {
    activate( cr - '0');	/* Relay on */
  }
  else if ( cr == '*' ) {
    measure();
  }
  else if ( cr == '@' ) {
    Serial.println("v0.1");
  }
  else if ( cr == '?' ) {
    Serial.println("a..h, realy off");
    Serial.println("A..H, realy on");
    Serial.println("*     return measurements");
    Serial.println("*       T for temp, H for humidity, C for CO2");
  }
  else {
    Serial.println("Not understood");
  }
  Serial.flush();
}

void loop() {
  decode(Serial.read());
}
