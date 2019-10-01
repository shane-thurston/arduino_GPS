#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define A_Lat -37.9067726   //sign
#define A_Lon 145.1351013
#define B_Lat -37.9067993  //hydrant
#define B_Lon 145.1345367

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
int redLeftPin = 4;
int greenOnPin = 5;
int redRightPin = 6;
int blueLEDPin = 9;
int greenLEDPin = 8;
int redLEDPin = 7;
int pA_Lat, pA_Lon, pB_Lat, pB_Lon;

//constant integers for directions
const int RIGHT = 1, LEFT = -1, ZERO = 0;

short directionOfPoint(int P_Lat, int P_Lon) 
{ 
    // subtracting co-ordinates of point A from 
    // B and P, to make A as origin 
    P_Lat -= pA_Lat; 
    P_Lon -= pA_Lon;  
  
    // Determining cross Product 
    int cross_product = (pB_Lat * P_Lon) - (pB_Lon * P_Lat); 
  
    // return RIGHT if cross product is positive 
    if (cross_product > 0) 
        return RIGHT; 
  
    // return LEFT if cross product is negative 
    if (cross_product < 0) 
        return LEFT; 
  
    // return ZERO if cross product is zero.  
    return ZERO;  
}

int lastFour(float num) {
  String s_num = String(num,7);
  int len = s_num.length();
  String last_4 = s_num.substring(len-4,len);
  int return_num = last_4.toInt();
  return return_num;
}

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  pinMode(redLeftPin, OUTPUT);
  pinMode(greenOnPin, OUTPUT);
  pinMode(redRightPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pA_Lat = lastFour(A_Lat);
  pA_Lon = lastFour(A_Lon);
  pB_Lat = lastFour(B_Lat);
  pB_Lon = lastFour(B_Lon);
  pB_Lat -= pA_Lat;
  pB_Lon -= pA_Lon;
}

void loop(){
  // This sketch displays information every time a new sentence is correctly encoded.
  /*int s_ALat = lastFour(A_Lat);
  Serial.println(s_ALat);
  int s_ALon = lastFour(A_Lon);
  Serial.println(s_ALon);
  int s_BLat = lastFour(B_Lat);
  Serial.println(s_BLat);
  int s_BLon = lastFour(B_Lon);
  Serial.println(s_BLon);*/
  
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      if (gps.satellites.value() <= 5 ) {
        analogWrite(redLEDPin, 255);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 0);
      }
      if ((gps.satellites.value() > 5 ) && (gps.satellites.value() <= 10 )){
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 255);
        analogWrite(blueLEDPin, 0);
      }
      if (gps.satellites.value() > 10 ) {
        analogWrite(redLEDPin, 0);
        analogWrite(greenLEDPin, 0);
        analogWrite(blueLEDPin, 255);
      }
      int P_Lat = lastFour(gps.location.lat());
      int P_Lon = lastFour(gps.location.lng());
      int direction = directionOfPoint(P_Lat, P_Lon); 
      if (direction == 1) 
          Serial.println("Right Direction"); 
      else if (direction == -1) 
          Serial.println("Left Direction"); 
      else
          Serial.println("Point is on the Line");
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 7);
      Serial.print(", Longitude= "); 
      Serial.print(gps.location.lng(), 7);
      // Number of satellites in use (u32)
      Serial.print(", Number satellites = "); 
      Serial.print(gps.satellites.value());
      Serial.print(", ALT=");  
      Serial.print(gps.altitude.meters());
      Serial.println("");
    }
  }
}
