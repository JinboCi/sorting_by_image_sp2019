# This project requires PyBluez
from tkinter import *
import bluetooth
import socket
from time import sleep
import serial
import pdb
import numpy as np

#some variables that need to be preset

'''
class ConnectToArduino
connect to the arduino with bluetooth and send data to it

usage example:
    a = ConnectToArduino("HC-06")
    a.SendData(dataArr, StartByte, InterStart, InterEnd, EndByte, WaitTime)

'''

class ConnectToArduino:
    deviceName = "MyBlueTooth"
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
            #print(bluetooth.lookup_name( i ))
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
        #sock.bind((self.MacAddr, port+1))        
        print ("Connected")
        return sock
     
    def __init__(self, name):
        self.deviceName = name
        tmp = self.FindMacAddr()
        if (tmp[0] == True):
            print(tmp[1])
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
        #pdb.set_trace()
        print("SendAndRead", Byte, ExpectedResponse)
        #.self.sock.accept()
        self.sock.send(Byte.encode())
        print("send", ord(list(Byte)[0]))
        #Response = self.sock.recv(1024)
        self.sock.settimeout(0.1)
        
        
        try:
            Response = self.sock.recv(1024)
        except socket.error:
            print("connection timed out!")
            return 0
        #pdb.set_trace()
        print("recv", list(Response)[0])
        print(list(Response)[-1])
        print(ord(list(ExpectedResponse)[0]))
        if (list(Response)[-1] == ord(list(ExpectedResponse)[0])):
            print("success")
            return 1 # success
        else:
            return 0 # failed

        
    
    '''
    function SendData:
        input: (dataArr[], StartByte, InterStart, InterEnd, EndByte, WaitTime)
    '''
    
    def SendData(self, dataArr, StartByte, InterStart, InterEnd, EndByte, positive, negative, WaitTime):
        ##print(dataArr)
        ##print(type(StartByte))
        print("SendData")
        while (1!=self.SendAndRead(StartByte, StartByte)):
            print("StartByte, StatByte")
            #sleep(WaitTime)     
        while (1!=self.SendAndRead(chr(len(dataArr)), chr(len(dataArr)))):
            print("len(dataArr), len(dataArr)")
            #sleep(WaitTime)
            #sleep(WaitTime) 
            '''
        while (1!=self.SendAndRead(StartByte, StartByte)):
            print("StartByte, StatByte")
            '''
        for idx in range(len(dataArr)):
            
            while (1!=self.SendAndRead(InterStart, InterStart)):
                print("InterStart, InterStart")
                #sleep(WaitTime)        
                
            if (dataArr[idx] < 0):
                while (1!=self.SendAndRead(negative, negative)):
                    print("ne", InterStart)
                    #sleep(WaitTime)
            else:
                while (1!=self.SendAndRead(positive, positive)):
                    print("po", positive)
                    #sleep(WaitTime)                
            while (1!=self.SendAndRead(chr(abs(dataArr[idx])), chr(abs(dataArr[idx])))):
                print("dataArr[idx]", idx, dataArr[idx])
                #sleep(WaitTime)
                
            while (1!=self.SendAndRead(InterEnd, InterEnd)):
                print("InterEnd, InterEnd")
                #sleep(WaitTime)
                
        self.sock.send(EndByte.encode())
            
            
StartByte = 121
InterStart = 122
InterEnd = 123
EndByte = 124
WaitTime = 0.01
positive = 125
negative = 126
dataArr = [1,2,3,4,5,6,7,8,9,10,20,30,40,60,80,-10,-20,-60,-90]
#arr = [1,2,3,4,5,6,7,8,9,10,20,30,40,60,80,110,130,150,170,190,200,210]
a = ConnectToArduino("MyBlueTooth")
#for i in arr:
#    a.SendAndRead(chr(i),chr(i))
a.SendData(dataArr, chr(StartByte), chr(InterStart), chr(InterEnd), chr(EndByte), chr(positive), chr(negative), WaitTime)
a.sock.close()