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

/******************************************
 ** Serial section
 ******************************************/
int readline(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
        break;
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}

void setup() {
  dht.begin();

  Serial.begin(9600);
}

void loop() {
  int co20;
  int co21;
  int co22;
  float h;
  float t;
  static char buffer[80];
  
  // Wait a few seconds between measurements.
  delay(1000);

  if (Serial.available() > 0) {
    if (readline(Serial.read(), buffer, 80) > 0) {
      Serial.print("You entered: >");
      Serial.print(buffer);
      Serial.println("<");
    }
  }

  /* Read the gas pin. */
  co20 = analogRead(0);
  /* Read the gas pin. */
  delay(1000); 
  co21 = analogRead(1);
  /* Read the gas pin. */
  delay(1000); 
  co22 = analogRead(2);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  delay(2000);
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("T");
  Serial.println(t);
  Serial.print("H");
  Serial.println(h);
  Serial.print("C0");
  Serial.println(co20);
  Serial.print("C1");
  Serial.println(co21);
  Serial.print("C2");
  Serial.println(co22);
}
