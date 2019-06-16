# This project requires PyBluez
from tkinter import *
import bluetooth
import socket
from time import sleep
import serial

#some variables that need to be preset

device_name = "HC-06"

def FindMacAddr(deviceName):
    #Look for all Bluetooth devices
    #the computer knows about.
    print ("Searching for devices...")
    #Create an array with all the MAC
    #addresses of the detected devices
    nearby_devices = bluetooth.discover_devices()
    #Run through all the devices found and list their name
    for i in nearby_devices:
    	if(deviceName==bluetooth.lookup_name( i )):
                print("find it")
                bd_addr = i
                return (True, bd_addr)
    print("bad name")
    return (False, "")

def ConnectDevice(MacAddr):
    #Allow the user to select their Arduino
    #bluetooth module. They must have paired
    #it before hand.
    print ("Starting a communication with", bluetooth.lookup_name(MacAddr))
    port = 1
    print ("Connecting to", bluetooth.lookup_name(MacAddr))
    sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    #s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock has the type bluetooth.msbt.BluetoothSocket
    sock.connect((MacAddr, port))
    print ("Connected")
    return sock


def FindAndConnect(diviceName):
    tmp = FindMacAddr(diviceName)
    if (tmp[0] == True):
        return ConnectDevice(tmp[1])
        
'''
function SendAndRead
input: 
    sock <class 'bluetooth.msbt.BluetoothSocket'>
    Byte: to be sent
    ExpectedResponse

'''
def SendAndRead(sock, Byte, ExpectedResponse):
    sock.send(Byte)
    Response = sock.read()
    if (Response == ExpectedResponse):
        return 1 # success
    else:
        return 0 # success
    

'''
function SendData:
    input: (sock, int dataArr[], StartByte, InterByte, EndByte, WaitTime)
'''

def SendData(sock, dataArr, StartByte, InterByte, EndByte, WaitTime):
    while (1!=SendAndRead(sock, StartByte, StartByte)):
        sleep(WaitTime)
    while (1!=SendAndRead(sock, len(dataArr), len(dataArr))):
        sleep(WaitTime)
    while (1!=SendAndRead(sock, InterByte, InterByte)):
        sleep(WaitTime)
    for idx in range(len(dataArr)):
        while (1!=SendAndRead(sock, dataArr[idx], dataArr[idx])):
            sleep(WaitTime)
        while (1!=SendAndRead(sock, InterByte, InterByte)):
            sleep(WaitTime)      
    while (1!=SendAndRead(sock, EndByte, EndByte)):
        sleep(WaitTime)
        
