import serial

with serial.Serial("/dev/ttyUSB0", 115200) as ser:
    while 1:
        print(ser.read(12))
