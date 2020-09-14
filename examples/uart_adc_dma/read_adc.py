import serial
from collections import Counter
from itertools import islice

def g(s):
    while 1:
        d = s.read(2)
        yield d[1] + 256*d[0]

with serial.Serial("/dev/ttyUSB0", 3000000) as ser:
    c = Counter(islice(g(ser), 1000, 1001000))

print(c.most_common())
