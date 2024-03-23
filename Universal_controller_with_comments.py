import network
import socket
import machine
import time
import utime
from machine import Pin, PWM

#function to control the motors and their speed
def go(sp1,sp2,sp3,sp4):
    if sp1 >= 0:
        dir1m1.on()
        dir2m1.off()
    else:
        dir1m1.off()
        dir2m1.on()
        
    if sp2 >= 0:
        dir1m2.on()
        dir2m2.off()
    else:
        dir1m2.off()
        dir2m2.on()
        
    if sp3 >= 0:
        dir1m3.on()
        dir2m3.off()
    else:
        dir1m3.off()
        dir2m3.on()
        
    if sp4 >= 0:
        dir1m4.on()
        dir2m4.off()
    else:
        dir1m4.off()
        dir2m4.on()
        
        
    EN1.duty(abs(sp1))
    EN2.duty(abs(sp2))
    EN3.duty(abs(sp3))
    EN4.duty(abs(sp4))

# Connect to WiFi
wifi_ap = network.WLAN(network.AP_IF)
wifi_ap.active(True)
wifi_ap.config(essid='UnKon', password='12345678')

# Create UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind(('0.0.0.0', 80))

#set all pins
EN1 = PWM(machine.Pin(4))
EN2 = PWM(machine.Pin(5))
EN3 = PWM(machine.Pin(18))
EN4 = PWM(machine.Pin(19))

dir1m1 = machine.Pin(13, machine.Pin.OUT)
dir2m1 = machine.Pin(12, machine.Pin.OUT)
dir1m2 = machine.Pin(14, machine.Pin.OUT)
dir2m2 = machine.Pin(27, machine.Pin.OUT)
dir1m3 = machine.Pin(26, machine.Pin.OUT)
dir2m3 = machine.Pin(25, machine.Pin.OUT)
dir1m4 = machine.Pin(33, machine.Pin.OUT)
dir2m4 = machine.Pin(32, machine.Pin.OUT)

go(0,0,0,0)

#Main loop
while True:
    data, addr = udp_socket.recvfrom(1024)  #Receive data from UDP
    command = data.decode('utf-8')
    print(command)
    
    #Remote control mode
    if command.startswith('R'):
        #Remove R letter from command
        command = command[1:]
        li = list(command.split("."))
        #Set speeds for every motor
        sp1 = int(li[0])
        sp2 = int(li[1])
        sp3 = int(li[2])
        sp4 = int(li[3])
        go(sp1,sp2,sp3,sp4)
        
    #Stop 
    elif command.startswith('S'):
        go(0,0,0,0)
        
    #Programming mode
    elif command.startswith('P'):
        command = command[1:-1]
        li = list(command.split("?"))
        for elem in li:
            params = list(elem.split("."))
            print(params, 'params')
            sp1 = params[0]
            sp2 = params[1]
            sp3 = params[2]
            sp4 = params[3]
            ti = params[4]
            print(sp1,sp2,sp3,sp4)
            go(int(sp1), int(sp2), int(sp3), int(sp4))
            time.sleep(int(ti))
