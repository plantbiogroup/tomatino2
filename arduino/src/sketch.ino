/* -*- mode: c -*- */
/* #include "DHT.h" */

#define RELAYS 8
#define DELAY 2000
#define ANALOG_PINS 6
#define DIGITAL_PINS 4
#define DHTTYPE DHT21   	/* DHT 21 (AM2301) */
#define RESTTIME 1000 * 60 	/* Time between dutycycle ms */
#define RELAYRESTTIME 1000	/* ms */

struct context {
  int analogsensorvalue[ANALOG_PINS];
  float humidity[DIGITAL_PINS];
  float temperature[DIGITAL_PINS];
  DHT *dht[DIGITAL_PINS];	/* Think about this one.... */
  int relay[RELAYS];
};

static struct context  ctx;

/******************************************
 ** Relay section
 ******************************************/
/* 3 is not working on test setup */
int relaymap[RELAYS]={2, 3, 4, 5, 6, 7, 8, 9};

void setrelay(int r, int val)
{
  if(val) digitalWrite(relaymap[r], LOW);
  else digitalWrite(relaymap[r], HIGH);
}
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


/******************************************
 ** MAIN section
 ******************************************/
void setup()
{
  int i;
  /* Set up the UART */
  Serial.begin(9600);

  /* Set up the relay pins */
  for(i=0; i<RELAYS; i++) {
    pinMode(ctx.relay[i], OUTPUT);
    setrelay(i, 0);
  }

  /* Set up the CO2 reader */

  /* Set up the AM3202 */

  /* Think about this one.... */
#define DHTPIN 2     // what pin we're connected to
  for(i=i; i<DIGITAL_PINS; i++) {
    ctx.dht[i] = new DHT(DHTPIN, DHTTYPE);
    ctx.dht[i]->begin();
  }
}

void loop()
{
  static char buffer[80];
  int i;

  delay(RESTTIME);
  /* Read the UART */
  if (readline(Serial.read(), buffer, 80) > 0) {
    Serial.print("You entered: >");
    Serial.print(buffer);
    Serial.println("<");
  }

  /*** Read all the analog pins ***/
  for(i=0; i<ANALOG_PINS; i++) {
    ctx.analogsensorvalue[i] = analogRead(i + A0);
  }

  /* Read humidity and temperature */         /* See ~/ws/plantbiogroup/tomatino/etc/DHT/examples/DHTtester/ */
  for(i=0; i<DIGITAL_PINS; i++) {
    delay(2000);		/* Wait between measurements */
    ctx.humidity[i] = ctx.dht[i]->readHumidity();
    ctx.temperature[i] = ctx.dht[i]->readTemperature();
  }

  /*** Set all the realys ***/
  for(i=0; i<RELAYS; i++) {
    delay(RELAYRESTTIME);
    setrelay(i, ctx.relay[i]);
  }

  /*** Report the values ***/
  for(i=0; i<ANALOG_PINS; i++) {
    printf("A%d %d\n", i, ctx.analogsensorvalue[i]);
  }
  for(i=0; i<DIGITAL_PINS; i++) {
    if (!isnan(ctx.humidity[i]) ) {
      printf("H%d %f\n", i, ctx.humidity[i]);
    }
  }
  for(i=0; i<DIGITAL_PINS; i++) {
    if (!isnan(ctx.temperature[i]) ) {
      printf("T%d %f\n", i, ctx.temperature[i]);
    }
  }
  for(i=0; i<RELAYS; i++) {
    printf("R%d %d\n", i, ctx.relay[i]);
  }
}
