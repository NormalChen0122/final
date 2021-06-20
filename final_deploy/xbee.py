import serial
import time

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

while (True):
    str = input("Enter St_forward distance(cm) to start(ex St_09): ")
    print("%s \n\r"%(str))
    # send RPC to remote
    s.write(("%s \n\r"%(str)).encode())
    time.sleep(1)
s.close()
