import serial
from collections import Counter
from itertools import islice


with serial.Serial("/dev/ttyUSB0", 3000000) as ser:
    c, = ser.read(1)
    while c != 0xff:
        c, = ser.read(1)
    while c == 0xff:
        c, = ser.read(1)
        d, e, f = ser.read(3)
        print((f << 24) + (e << 16) + (d << 8) + c)
    for i in range(10):
        c, d, e, f = ser.read(4)
        c, d, e, f = ser.read(4)
        print((f << 24) + (e << 16) + (d << 8) + c)
