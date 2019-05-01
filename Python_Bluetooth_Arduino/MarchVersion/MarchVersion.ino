//Include the module so we don't
//have to use the default Serial
//so the Arduino can be plugged in
//to a computer and still use bluetooth
#include <SoftwareSerial.h>

//Define the pins used for receiving
//and transmitting information via Bluetooth
const int rxpin = 0;
const int txpin = 1;
const int pin1 = 7;
const int pin2 = 8;
const int pin3 = 12;
const int delay_time = 100;
//Variable to store input value
//initialized with arbitrary value
byte num = 0;
unsigned char k = num;
//data to store values
char inData[128];

//three angles
unsigned angle1 = 0;
unsigned angle2 = 0;
unsigned angle3 = 0;
unsigned max_angle = 0;

//bools to indicate whether headers are recieved
bool header0 = false;
bool header1 = false;
bool header2 = false;
bool header3 = false;
bool ang1_rev = false;
bool ang2_rev = false;
bool ang3_rev = false;

//Connect the Bluetooth module
SoftwareSerial bluetooth(rxpin, txpin);

//Define the pin to control the light
int lightbulb = 7;

void setup()
{
  //Set the pin to put power out
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  //Initialize Serial for debugging purposes
  Serial.begin(9600);
  Serial.println("Serial ready");
  //Initialize the bluetooth
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth ready");
  Serial.println("Bluetooth ready");
}

void loop()
{
  //Check for new data
  if(bluetooth.available()){
    //Remember new data
    k = bluetooth.read();
    //Print the data for debugging purposes
    // Serial.println(k);
  }
  
  num = k;
  Serial.println(num);
  if ((bluetooth.available()) && (num >= 150)) {
    header0 = true;
    Serial.println("header0 recieved");
    for(int i =0; i < 128; i++){
      inData[i] = bluetooth.read();
      delay(1);
      
      }
      /*
    for(int i = 0; i < 128; i++)
    {
      Serial.print((byte)inData[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    delay(1000);
    */
    
    for(int i = 0; i < 128; i++) {
      /*
      Serial.print("inData:");
      Serial.println((byte)inData[i]);
      delay(100);
      */
      byte d = inData[i];
      
      if ((header1==false) && (150 <= d)) {
        header1 = true;
        Serial.println("header1 recieved");
        
      }
      else if ((header1 == true) &&(ang1_rev == false) && (100 >= d) && (0 != d)) {
        angle1 = (unsigned)d;
        ang1_rev = true;
        Serial.print("angle1 recieved: ");
        Serial.println(angle1);
        continue;
      }
      else if (150 <= d && (header2==false)) {
        header2 = true;
        Serial.println("header2 recieved");
        continue;
      }
      else if ((header2 == true) && (ang2_rev == false) && (100 >= d) && (0 != d)) {
        angle2 = (unsigned)d;
        ang2_rev = true;
        Serial.print("angle2 recieved: ");
        Serial.println(angle2);
        continue;
      }
      else if (150 <= d && (header3==false)) {
        header3 = true;
        Serial.println("header3 recieved");
        continue;
      }
      else if ((header3 == true) && (ang3_rev == false) && (100 >= d)&& (0 != d)) {
        angle3 = (unsigned)d;
        ang3_rev = true;
        Serial.print("angle3 recieved: ");
        Serial.println(angle3);        
        continue;
      }
    }
  
  
  
    if (ang1_rev && ang2_rev && ang3_rev) { 
      // using delay() to create pwm with lower frequency.
      if(angle3 > 0) {angle3 = angle3-1;}
      if(angle1 > 0) {angle3 = angle3-1;}
      if(angle2 > 0) {angle3 = angle3-1;}

      
      if (angle3 > angle1) {
        max_angle = angle3;
        if (angle2 > max_angle) {
          max_angle = angle2;
        }
      } else
      {
        max_angle = angle1;
        if (angle3 > max_angle) {
          max_angle = angle2;
        }
      }
      for(unsigned i = 0; i < max_angle; i++)
      {
        if (i < angle1) {
          digitalWrite(pin1, HIGH);
        } else
        {
          digitalWrite(pin1, LOW);
        }
        if (i < angle2) {
          digitalWrite(pin2, HIGH);
        } else
        {
          digitalWrite(pin2, LOW);
        }
        if (i < angle3) {
          digitalWrite(pin3, HIGH);
        } else
        {
          digitalWrite(pin3, LOW);
        }
        delay(delay_time);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        delay(delay_time);
      }
    } else
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        delay(2*delay_time);
    }
  }
  else {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
        digitalWrite(pin3, LOW);
        delay(2*delay_time);
  }
  
  
}
