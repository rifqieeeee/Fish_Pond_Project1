// https://randomnerdtutorials.com/esp32-k-type-thermocouple-max6675/
// https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/
// https://www.electroniclinic.com/esp32-ph-sensor-iot-ph-sensor-code-and-circuit-diagram/

#include "MAX6675.h"

//#1
const int trigPin = 13;
const int echoPin = 12;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;

//#2
// note: pins are slightly different than other examples!
const int dataPin   = 19;
const int clockPin  = 5;
const int selectPin = 23;
MAX6675 thermoCouple;
uint32_t start, stop;

//#3
float calibration_value = 20.24 - 0.7; //21.34 - 0.7
int phval = 0; 
unsigned long int avgval;
int buffer_arr[10],temp;
float ph_act;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Starts the serial communication
  //#1
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //#2
  thermoCouple.begin(clockPin, selectPin, dataPin);
  thermoCouple.setSPIspeed(4000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //#1
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  //#2
  start = micros();
  int status = thermoCouple.read();
  stop = micros();
  float temp = thermoCouple.getTemperature();

  //#3
  for(int i=0;i<10;i++){ 
    buffer_arr[i]=analogRead(15);
    delay(30);
  }
  for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(buffer_arr[i]>buffer_arr[j]){
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }
  avgval=0;
  for(int i=2;i<8;i++)
    avgval+=buffer_arr[i];
  float volt=(float)avgval*3.3/4096.0/6;  
  ph_act = (-5.70 * volt + calibration_value) * 10;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.print(distanceCm);
  Serial.print("\ttemp: ");
  Serial.print(temp);
  Serial.print("\tpH Val: ");
  Serial.println(ph_act);
  
  delay(1000);
}
