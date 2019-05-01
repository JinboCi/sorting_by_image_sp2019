#include <SoftwareSerial.h>


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
  
  //Connect the Bluetooth module
  SoftwareSerial bluetooth(rxpin, txpin);
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
  SoftwareSerial bluetooth = bluetoothsetup(0, 1);
  /* TODO */

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
  if (!bluetooth.available()) {
    return 0;
  }
   byte buff = bluetooth.read();
   // when we cannot get the StartKey, keep recieving
   while (StartKey != buff){
     buff = bluetooth.read();
     delay(1); // I do not know whether this step is necessary
     bluetooth.write(buff); // send response
   }
   byte data = buff;
   while (EndKey != buff){
      buff = bluetooth.read();
      data = buff;
      delay(1); // I do not know whether this step is necessary
      bluetooth.write(data); // send response
   }
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
    int fai[1];
    fai[0] = 0;
    return fai;
  }
  int arr[buff+1];
  for (byte i = 0; i < buff ; i++){
    buff = receiveByte(bluetooth, InterStart, InterEnd);
    if(buff == 0) {
      Serial.println((int)i, "th Receive Failed");
      arr[0] = 0;
      break;
    }
    if (buff == EndByte) {
      Serial.println((int)i, "End Unexpectedly at");
      arr[0] = i+1;
      break;
    }
    arr[i+1] = (int)buff;
    buff = receiveByte(bluetooth, InterEnd, InterEnd);
  }
  return arr;
}
  


void loop() {
  // put your main code here, to run repeatedly:

}
