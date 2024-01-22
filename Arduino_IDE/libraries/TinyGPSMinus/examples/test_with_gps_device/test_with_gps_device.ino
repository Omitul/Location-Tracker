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

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPSMinus &gps);
static void print_str(const char *str, int len);

void setup() {
  Serial.begin(115200);
  
  Serial.println(F("Testing TinyGPSMinus static object"));
  Serial.print(F("Sizeof(gpsobject) = "));
  Serial.print(sizeof(TinyGPSMinus));
  Serial.println(F(" bytes"));
  Serial.println();

  Serial.println(F("Ambiguity HDOP Latitude Longitude Fix  Date       Time       Date Alt     Course Speed Card  Chars Sentences Checksum"));
  Serial.println(F("(m)            ddmm.hhN dddmm.hhW Age                        Age  (m)     --- from GPS ----  RX    RX        Fail"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------"));
  
  ss.begin(4800);
}

void loop() {
  char lat[9], lon[10];
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  
  print_float(gps.ambiguity(), -1, 10, 2);
  print_int(gps.hdop(), TinyGPSMinus::GPS_INVALID_HDOP, 5);
  strcpy(lat, gps.get_latitude());
  strcpy(lon, gps.get_longitude());
  gps.get_pos_age(&age);
  print_str(lat, 9);
  print_str(lon, 10);
  print_int(age, TinyGPSMinus::GPS_INVALID_AGE, 5);
  print_date(gps);
  print_float(gps.f_altitude(), TinyGPSMinus::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(gps.f_course(), TinyGPSMinus::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPSMinus::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPSMinus::GPS_INVALID_F_ANGLE ? "*** " : TinyGPSMinus::cardinal(gps.f_course()), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(1000);
}

static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec) {
  if (val == invalid) {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  } else {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len) {
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPSMinus &gps) {
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPSMinus::GPS_INVALID_AGE)
    Serial.print(F("********** ******** "));
  else {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ", month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPSMinus::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len) {
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
