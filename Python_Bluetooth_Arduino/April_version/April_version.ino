#include <SoftwareSerial.h>

const int rxpin = 2;
const int txpin = 3;
const int StartByte = 100;
const int InterStart = 101;
const int InterEnd = 102;
const int EndByte = 103;
const int positive = 104;
const int negative = 105;
//Connect the Bluetooth module
SoftwareSerial bluetooth(rxpin, txpin);
/* function bluetoothsetup 
 *  setup bluetooth
 *  should be called at setup()
 *  
 *  inputs: const int rxpin, const int txpin //Define the pins used for receiving
 *  outputs: bluetooth with type SoftwareSerial
 *  
 *  
 *  
 *  
 */
SoftwareSerial bluetoothsetup(const int rxpin, const int txpin){
  

  //Initialize Serial for debugging purposes
  Serial.begin(9600);
  Serial.println("Serial ready");
  //Initialize the bluetooth
  bluetooth.begin(9600);
  Serial.println("Bluetooth ready");
  return bluetooth;
}



  
 
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
  /*
 StartByte = 255;
 InterStart = 254;
 InterEnd = 253;
 EndByte = 252;
 */

}



/*
 * function receiveByte
 * return the data between the StartKey and the EndKey
 * 
 * inputs: SoftwareSerial bluetooth, byte StartKey, byte EndKey
 * outputs byte that we recieved or 0 for failure
 * 
 * 
 */

byte receiveByte (SoftwareSerial bluetooth, byte StartKey, byte EndKey){
  // if not available, return 0 for failure
  Serial.println("receiveByte");
  
  //while (!bluetooth.available()) {
    //Serial.println("no data inputs");
    //delay(100);
  //}
  Serial.print("available: ");
  Serial.println(bluetooth.available());
  Serial.print("StartKey: ");
  Serial.println(StartKey);
   byte buff = bluetooth.read();
        Serial.print("buff: ");
   Serial.println(buff);
   // when we cannot get the StartKey, keep recieving
   // wait until we get the start key
   do {
      while (!bluetooth.available()) {
    Serial.println("no data inputs");
    delay(1000);
  }
    
     buff = bluetooth.read();
     delay(1000); // I do not know whether this step is necessary
     Serial.print("loop1Write: ");
     //Serial.println(buff);
     Serial.println(buff);
     
     bluetooth.write(buff); // send response
    
   }while (StartKey != buff);
   // continue telling the host that we get the start key, until it responses
   while (StartKey == buff){
        //Serial.print(bluetooth.read());
      while (!bluetooth.available()) {
    Serial.println("no data inputs");
    delay(1000);
  }
     buff = bluetooth.read();
     delay(1000); 
     Serial.print("loop2 write: ");
     //Serial.println(buff);
     Serial.println(buff);
     bluetooth.write(buff); // send response
    }
    
   
   // receive data
   Serial.println("receive data");
   byte data = buff;
   Serial.println(data);
     Serial.print("EndKey: ");
  Serial.println(EndKey);
   Serial.print("buff: ");
   Serial.println(buff);
   do{
        while (!bluetooth.available()) {
    Serial.println("no data inputs");
    delay(1000);
  }
        buff = bluetooth.read();
      
      Serial.print("loop3writebuff: ");
      Serial.println(buff);
      //delay(1000); // I do not know whether this step is necessary
      Serial.print("data: ");
      //Serial.print(data);
      //Serial.print(" ");
      Serial.println(data);
      bluetooth.write(buff); // send response
      
   }while (EndKey != buff);
   // continue telling the host that we get the end key, until it responses
   while (EndKey == buff){
      while (!bluetooth.available()) {
    Serial.println("no data inputs");
    delay(1000);
  }
     buff = bluetooth.read();
     delay(1000); 
     Serial.print("loop4writebuff: ");
     //Serial.println(buff);
     Serial.println(buff);
     bluetooth.write(buff); // send response
    }
   
   
   Serial.println("end");
   
   return data;
  }


/*
 * function receiveData
 * 
 * inputs: SoftwareSerial bluetooth, byte StartByte, byte InterStart, 
 *         byte InterEnd, byte EndByte, int WaitTime
 * outputs: an int array with array[0] is the total length of the array
 * 
 * attentions:
 * 1) the data would be sent from the computer in the following order:
 * StartByte - Number - InterStart - First Data - InterEnd - 
 * InterStart - Second Data - InterEnd - ... - InterEnd - EndByte
 * 2) the computer would not send the second data, until it recieve the response
 */


int * receiveData(SoftwareSerial bluetooth, byte StartByte, byte InterStart, byte InterEnd, byte EndByte){
  //Check for new data
  byte buff = receiveByte(bluetooth, StartByte, InterStart);
  if(buff == 0) {
    Serial.println("Num Receive Failed");
    int * fai = new int[1];
    fai[0] = 0;
    return fai;
  }
  int * arr = new int[buff+1];
  for (byte i = 0; i < buff ; i++){
    buff = receiveByte(bluetooth, InterStart, InterEnd);
    if(buff == 0) {
      Serial.print(i);
      Serial.println("th Receive Failed");
      arr[0] = 0;
      break;
    }
    if (buff == EndByte) {
      Serial.print(i);
      Serial.println("End Unexpectedly at");
      arr[0] = i+1;
      break;
    }
    arr[i+1] = (int)buff;
    buff = receiveByte(bluetooth, InterEnd, InterEnd);
  }
  return arr;
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
  /*
  Serial.print("available: ");
  Serial.println(bluetooth.available());
  
  //if(bluetooth.available()){
    //Serial.println(bluetooth.read());
  int number_wanted = 4;
  //SoftwareSerial bluetooth = bluetoothsetup(0, 1);
  Serial.println("start");
  int * res = receiveData(bluetooth, StartByte, InterStart, InterEnd, EndByte);
  if (res[0] != number_wanted)
  {
    Serial.println("the num of data recieved does not match with numberwanted");
    delay(1000);
  } else {
  Serial.println("the num of data recieved matches with numberwanted");
  for (int i = 0; i <= res[0]; i ++){
    Serial.print(res[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(1000);
  }
  //}
  
  */
  // start receiving data
  byte time_de= 100;
  byte buff = bluetooth.read();
  int num = 0;
  int sign = 1;
  if (buff == StartByte) {
    Serial.println((byte)buff); 
    bluetooth.write(buff);
    buff = readoff(StartByte, time_de);
    //buff = bluetooth.read();
    num = buff;
    Serial.println(num);
    bluetooth.write(buff);
    buff = readoff(num, time_de);
    
    
    
    int data[num];
    for(int i = 0; i < num; i++) {
        buff = readoff(InterStart, time_de);
        Serial.print("Sign: ");
        Serial.println(buff);
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
        Serial.println(data[i]); 
        bluetooth.write(data[i]);  
        buff = readoff(dataabs, time_de);
        buff = readoff(InterEnd, time_de);
 
    }
    Serial.println("Result:");
    for(int i = 0; i < num; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
  }
  delay(time_de);
  /*
      char c;  
     if (bluetooth.available())  
     {  
       c = bluetooth.read();  
        
       Serial.println((byte)c);             // Write the character to the Serial Monitor  
        
       bluetooth.write (c);           // Write the character to Bluetooth  
     } 
  
delay(1000);
*/
}
