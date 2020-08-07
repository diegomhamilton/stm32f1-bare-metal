import serial
from collections import Counter
from itertools import islice


with serial.Serial("/dev/ttyUSB0", 3000000) as ser:
    c = Counter(islice(g(ser), 1000, 1001000))

