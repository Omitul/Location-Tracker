#include <SoftwareSerial.h>

#include <TinyGPSMinus.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   serial GPS device hooked up.
*/

#define RXPIN 14 // D14 = A0 on the Arduino Nano
#define TXPIN -1 // unused
#define GPSBAUD 9600

TinyGPSMinus gps;
SoftwareSerial ss(RXPIN, TXPIN);

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBAUD);
  
  Serial.println(F("Testing TinyGPSMinus static object"));
  Serial.print(F("Sizeof(gpsobject) = "));
  Serial.print(sizeof(TinyGPSMinus));
  Serial.println(F(" bytes"));
  Serial.println();
}

void loop() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (ss.available()) {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData) {
    char lat[9], lon[10];
    unsigned long age;
    strcpy(lat, gps.get_latitude());
    strcpy(lon, gps.get_longitude());
    gps.get_pos_age(&age);
    Serial.print(F("LAT="));
    Serial.print(lat);
    Serial.print(F(" LON="));
    Serial.print(lon);
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(F(" CHARS="));
  Serial.print(chars);
  Serial.print(F(" SENTENCES="));
  Serial.print(sentences);
  Serial.print(F(" CSUM ERR="));
  Serial.println(failed);
  if (chars == 0)
    Serial.println(F("** No characters received from GPS: check wiring **"));
}