# This project requires PyBluez
from tkinter import *
import bluetooth
import socket
from time import sleep
import serial

#some variables that need to be preset
device_name = "HC-06"
#you may need to change the data
#the duty cycle = (data -32)/94

data = chr(255)
header1 = chr(254)
header2 = chr(253)
header3 = chr(252)
times = 5
interval = 0.01
original_ang1 = 0.0
expected_ang1 = 30.0
peak_needed1 = chr(int((expected_ang1-original_ang1)/1.8)+1)

original_ang2 = 0.0
expected_ang2 = 30.0
peak_needed2 = chr(int((expected_ang2-original_ang2)/1.8)+1)

original_ang3 = 0.0
expected_ang3 = 30.0
peak_needed3 = chr(int((expected_ang3-original_ang3)/1.8)+1)

#Look for all Bluetooth devices
#the computer knows about.
print ("Searching for devices...")
print ("")
#Create an array with all the MAC
#addresses of the detected devices
nearby_devices = bluetooth.discover_devices()
#Run through all the devices found and list their name
for i in nearby_devices:
	if(device_name==bluetooth.lookup_name( i )):
            print("find it")
            print(type(i))
            bd_addr = i
            break

#Allow the user to select their Arduino
#bluetooth module. They must have paired
#it before hand.
print ("Starting a communication with", bluetooth.lookup_name( bd_addr ))

port = 1
print ("Connecting to", bluetooth.lookup_name( bd_addr ))
sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
print(type(sock))
#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((bd_addr, port))
print ("Connected")
#print ("The duty Cycle should be:",(data_n -32)/94*100, "%" )

for i in range(times):
    sock.send(data)
    sleep(interval)
    sock.send(header1)
    sleep(interval)
    sock.send(peak_needed1)
    sleep(interval)
    sock.send(header2)
    sleep(interval)
    sock.send(peak_needed2)
    sleep(interval)
    sock.send(header3)
    sleep(interval)
    sock.send(peak_needed3)
    sleep(interval)

for i in range(10):
    sock.send(chr(0))
    sleep(interval*10)
sock.close()
print("Done")
