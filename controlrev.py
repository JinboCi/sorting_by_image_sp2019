# This project requires PyBluez
from tkinter import *
import bluetooth
import socket
from time import sleep
import serial

#some variables that need to be preset

'''
class ConnectToArduino
connect to the arduino with bluetooth and send data to it

usage example:
    a = ConnectToArduino("HC-06")
    a.SendData(dataArr, StartByte, InterStart, InterEnd, EndByte, WaitTime)

'''

class ConnectToArduino:
    deviceName = "HC-06"
    MacAddr = ""
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    
    def SetName(self, newName):
        self.deviceName = newName
        
    def FindMacAddr(self):
        #Look for all Bluetooth devices
        #the computer knows about.
        print ("Searching for devices...")
        #Create an array with all the MAC
        #addresses of the detected devices
        nearby_devices = bluetooth.discover_devices()
        #Run through all the devices found and list their name
        for i in nearby_devices:
        	if(self.deviceName==bluetooth.lookup_name( i )):
                    print("find it")
                    bd_addr = i
                    return (True, bd_addr)
        print("bad name")
        return (False, "")
    
    def ConnectDevice(self):
        #Allow the user to select their Arduino
        #bluetooth module. They must have paired
        #it before hand.
        print ("Starting a communication with", bluetooth.lookup_name(self.MacAddr))
        port = 1
        print ("Connecting to", bluetooth.lookup_name(self.MacAddr))
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        #s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # sock has the type bluetooth.msbt.BluetoothSocket
        sock.connect((self.MacAddr, port))
        print ("Connected")
        return sock
     
    def __init__(self, name):
        self.deviceName = name;
        tmp = self.FindMacAddr()
        if (tmp[0] == True):
            self.MacAddr = tmp[1]
            self.sock = self.ConnectDevice()
            
    '''
    function SendAndRead
    input: 
        Byte: to be sent
        ExpectedResponse
    
    '''
    def SendAndRead(self, Byte, ExpectedResponse):
        ##Byte = chr(Byte)
        ##print(type(Byte))
        ##print(Byte)

        self.sock.send(Byte)
        ExpectedResponse = chr(ExpectedResponse)
        Response = chr(self.sock.read())
        if (Response == ExpectedResponse):
            return 1 # success
        else:
            return 0 # failed

        
    
    '''
    function SendData:
        input: (dataArr[], StartByte, InterStart, InterEnd, EndByte, WaitTime)
    '''
    
    def SendData(self, dataArr, StartByte, InterStart, InterEnd, EndByte, WaitTime):
        ##print(dataArr)
        ##print(type(StartByte))
        while (1!=self.SendAndRead(StartByte, InterStart)):
            sleep(WaitTime)
        while (1!=self.SendAndRead(len(dataArr), len(dataArr))):
            sleep(WaitTime)
        while (1!=self.SendAndRead(InterStart, InterEnd)):
            sleep(WaitTime)
        for idx in range(len(dataArr)):
            while (1!=self.SendAndRead(dataArr[idx], dataArr[idx])):
                sleep(WaitTime)
            while (1!=self.SendAndRead(InterStart, InterEnd)):
                sleep(WaitTime)      
        while (1!=self.SendAndRead(EndByte, EndByte)):
            sleep(WaitTime)
            
