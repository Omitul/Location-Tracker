#include <TinyGPSMinus.h>

/* This sample code demonstrates the basic use of a TinyGPSMinus object.
   Typically, you would feed it characters from a serial GPS device, but 
   this example uses static strings for simplicity.
*/
const char str1[] PROGMEM = "$GPRMC,201547.000,A,3014.5527,N,09749.5808,W,0.24,163.05,040109,,*1A";
const char str2[] PROGMEM = "$GPGGA,201548.000,3014.5529,N,09749.5808,W,1,07,1.5,225.6,M,-22.5,M,18.8,0000*78";
const char str3[] PROGMEM = "$GPRMC,201548.000,A,3014.5529,N,09749.5808,W,0.17,53.25,040109,,*2B";
const char str4[] PROGMEM = "$GPGGA,201549.000,3014.5533,N,09749.5812,W,1,07,1.5,223.5,M,-22.5,M,18.8,0000*7C";
const char *teststrs[4] = {str1, str2, str3, str4};

static void sendstring(TinyGPSMinus &gps, const PROGMEM char *str);
static void gpsdump(TinyGPSMinus &gps);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPSMinus &gps);
static void print_str(const char *str, int len);

void setup() {
  TinyGPSMinus test_gps;
  Serial.begin(115200);

  Serial.println(F("Testing TinyGPSMinus static object"));
  Serial.print(F("Sizeof(gpsobject) = "));
  Serial.print(sizeof(TinyGPSMinus));
  Serial.println(F(" bytes"));
  Serial.println();

  Serial.println(F("Ambiguity HDOP Latitude Longitude Fix  Date       Time       Date Alt     Course Speed Card  Chars Sentences Checksum"));
  Serial.println(F("(m)            ddmm.hhN dddmm.hhW Age                        Age  (m)     --- from GPS ----  RX    RX        Fail"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------"));
  
  gpsdump(test_gps);

  for (char i = 0; i < 4; ++i) {
    sendstring(test_gps, teststrs[i]);
    gpsdump(test_gps);
  }
}

void loop() {
}

static void sendstring(TinyGPSMinus &gps, const PROGMEM char *str) {
  while (true) {
    char c = pgm_read_byte_near(str++);
    if (!c) break;
    gps.encode(c);
  }
  gps.encode('\r');
  gps.encode('\n');
}

static void gpsdump(TinyGPSMinus &gps) {
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

  print_float(gps.f_altitude(), TinyGPSMinus::GPS_INVALID_F_ALTITUDE, 8, 2);
  print_float(gps.f_course(), TinyGPSMinus::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPSMinus::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPSMinus::GPS_INVALID_F_ANGLE ? "*** " : TinyGPSMinus::cardinal(gps.f_course()), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
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
}

static void print_float(float val, float invalid, int len, int prec) {
  char sz[32];
  if (val == invalid) {
    strcpy(sz, "*******");
    sz[len] = 0;
        if (len > 0) 
          sz[len-1] = ' ';
    for (int i=7; i<len; ++i)
      sz[i] = ' ';
    Serial.print(sz);
  } else {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1);
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(" ");
  }
}

static void print_date(TinyGPSMinus &gps) {
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPSMinus::GPS_INVALID_AGE)
    Serial.print(F("*******    *******    "));
  else {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d   ", month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPSMinus::GPS_INVALID_AGE, 5);
}

static void print_str(const char *str, int len) {
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
}