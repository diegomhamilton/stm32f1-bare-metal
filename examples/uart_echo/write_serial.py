import serial
import time

port_name = "/dev/ttyUSB0"
with serial.Serial(port_name, 115200) as ser:
    print("Writing on the serial port {}".format(port_name))
    for n in range(1000):
        ser.write(b"Testing...\r\n")
        print("{:3d} ".format(n + 1), end=" ", flush=True)
        if (n + 1) % 40 == 0:
            print()
        time.sleep(1)
