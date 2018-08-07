import serial
import time
import sys
import re
import argparse

print('open serial port')

ser = serial.Serial(port='/dev/ttyS0', baudrate=9600, timeout=1)

def senCommand(ser, data):
    ser.write((data+'\r').encode())
    a = ser.read(100)
    return a

parser = argparse.ArgumentParser(
    prog = 'Read and Write Serial Port',
    usage = 'Move Stage',
    description = 'help',
    epilog = 'end',
    add_help = True,
    )

parser.add_argument('-o', '--origin', help='back to the origin', nargs=0)
parser.add_argument('-r', '--reset', help='reset data', action='store_true')
parser.add_argument('-mr', '--moveright', help='move right by number of pulses', nargs=1, type=int)
parser.add_argument('-ml', '--moveleft', help='move left by number of pulses', nargs=1, type=int)
parser.add_argument('-pr', '--positionright', help='move right to the cordinate of the pulse', nargs=1, type=int)
parser.add_argument('-pl', '--positionleft', help='move left to the cordinate of the pulse', nargs=1, type=int)
parser.add_argument('-s', '--stop', help='stop the action', action='append', nargs=1)
parser.add_argument('-q', '--q', help='return the data of stage', action='store_true')


args = parser.parse_args()
#print(args)

if args.origin:
    c1 = "H:1+"
    c2 = "M:1+P40000"
    c3 = "G:"
    c4 = "R:1"
    read = sendCommand(ser, c1) + sendCommand(ser, c2) + sendCommand(ser, c3) + sendCommand(ser, c4)
    print(read)

elif args.reset:
    c1 = "R:1"
    read = sendCommand(ser, c1)
    print(read)

elif args.moveright:
    c1 = "M:1+P" + str(args.moveright[0])
    print(c1)
    read = sendCommand(ser, c1) + sendCommand(ser, "G:")
    print(read)

elif args.moveleft:
    c1 = "M:1-P" + str(args.moveleft[0])
    print(c1)
    read = sendCommand(ser, c1) + sendCommand(ser, "G:")
    print(read)

elif args.positionright:
    c1 = "A:1+P" + str(args.actionright[0])
    read = sendCommand(ser, c1) + sendCommand(ser, "G:")
    print(read)

elif args.positionleft:
    c1 = "A:1-P" + str(args.actionleft[0])
    read = sendCommand(ser, c1) + sendCommand(ser, "G:")
    print(read)

elif args.stop:
    c1 = "L:1"
    read = sendCommand(ser, c1)
    print(read)

elif args.q:
    c1 = "Q:"
    read = sendCommand(ser, c1)
    print(read)

else:
    print("Error: No such Command!!")

ser.close()
