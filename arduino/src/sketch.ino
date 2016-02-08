/* -*- mode: c -*- */

void setup()
{
  int i;
  /* Set up the UART */
  Serial.begin(9600);
}

void loop()
{
  delay(1000);
  Serial.println(".");
}
