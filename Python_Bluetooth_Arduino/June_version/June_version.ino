#include <SoftwareSerial.h>

const int rxpin = 2;
const int txpin = 3;
const int StartByte = 121;
const int InterStart = 122;
const int InterEnd = 123;
const int EndByte = 124;
const int positive = 125;
const int negative = 126;
const int time_de= 1;
//Connect the Bluetooth module
SoftwareSerial bluetooth(rxpin, txpin);

void setup()
{
  //Set the pin to put power out
  //SoftwareSerial bluetooth(rxpin, txpin);
  //Initialize Serial for debugging purposes
  Serial.begin(9600);
  Serial.println("Serial ready");
  //Initialize the bluetooth
  bluetooth.begin(9600);
  Serial.println("Bluetooth ready");

}


byte readoff(byte data, int wait_time) {
  byte buff;
  
  while (true) {
    while (!bluetooth.available()){
      delay(wait_time);
    }
    buff = bluetooth.read();
    if (buff == data){
      bluetooth.write(data);
      buff = bluetooth.read();
      delay(wait_time);
    } else {
      break;
    }
  }
  return buff;
}

void loop() {
  // put your main code here, to run repeatedly:

  // start receiving data
  
  byte buff = bluetooth.read();
  int num = 0;
  int sign = 1;
  if (buff == StartByte) {
    //Serial.println((byte)buff); 
    bluetooth.write(buff);
    buff = readoff(StartByte, time_de);
    //buff = bluetooth.read();
    num = buff;
    //Serial.println(num);
    bluetooth.write(buff);
    buff = readoff(num, time_de);
    
    
    
    int data[num];
    for(int i = 0; i < num; i++) {
        buff = readoff(InterStart, time_de);
        //Serial.print("Sign: ");
        //Serial.println(buff);
        sign = buff;
        bluetooth.write(sign);
        
        buff = readoff(sign, time_de);
        if (sign == negative) {
          sign = -1;
        } else {
          sign = 1;
        }
        int dataabs = buff;
        data[i] = sign*dataabs;
        //Serial.println(data[i]); 
        bluetooth.write(data[i]);  
        buff = readoff(dataabs, time_de);
        buff = readoff(InterEnd, time_de);
 
    }
    // print out what we have received to Serial
    Serial.println("data:");
    for(int i = 0; i < num; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  delay(time_de);
}
