#!/usr/bin/env python3
import serial
import time
import sys
import re

args = sys.argv

print('open serial port')

def sendCommand(ser, data):
    ser.write( (data+'\r').encode())
    a=ser.read(100)
    return a


args = sys.argv
parser = argparse.ArgumentParser(
    prog=args[0],
    usage="Move Stage",
    description="HELP",
    epilog="END",
    add_help=True,
    )


ser = serial.Serial(port='/dev/ttyS0', baudrate=9600, timeout=1)


regex = r"%s"%args[1]

t = 0

sym = r"[-](o|r|m|a|l|q|h)"
check = re.compile(sym)
matchObj = check.match(regex)

if matchObj:
    t = t
else:
    t = t + 1
    print ("Error:No Such Command")

if t==0:
    pattern = re.compile(regex)
    matchObj = pattern.match("-h")
    if matchObj:
        print (matchObj.group())
        regex2 = r"%s"%args[2]
        n = r"[+]|[-]"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "H:"+"1"
            c2 = args[2]
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \' +/- \'")

    matchObj = pattern.match("-r")
    if matchObj:
        print (matchObj.group())
        c1 = "R:1"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

    matchObj = pattern.match("-m")
    if matchObj:
        print (matchObj.group())
        regex2 = r"%s"%args[2]
        n = r"[+]P\d|[-]P\d"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "M:"+'1'+args[2] 
            c2 = "G:"
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \'+/- P [Number]\'")

    matchObj = pattern.match("-a")
    if matchObj:
        print (matchObj.group())
        regex2 = r"%s"%args[2]
        n = r"[+]P\d|[-]P\d"
        num = re.compile(n)
        matchNum = num.match(regex2)
        if matchNum:
            c1 = "A:"+"1"+args[2]
            c2 = "G:"
            read = sendCommand(ser, c1) + sendCommand(ser, c2)
            print(c1+c2)
            print(read)
        else:
            print("Error: You should enter \'+/- P [Number]\'")

    matchObj = pattern.match("-l")
    if matchObj:
        print (matchObj.group())
        c1 = "L:"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

    matchObj = pattern.match("-q")
    if matchObj:
        print (matchObj.group())
        c1 = "Q:"
        read = sendCommand(ser, c1)
        print(c1)
        print(read)

ser.close()

#print(ser.name)
#print(a)
#ser.write("Q:".encode())
#print('Write Q')
#a = sendCommand(ser, 'Q:')
#print(a)

#regex = r"M:"
#pattern = re.compile(regex)
#text = "M:"
#matchObj = re.match(pattern, text)
#if matchObj:
#	print (matchObj.group())

sys.exit(0)

while False:
    c = input('Enter command>')
    if c == 'quit':
        break
    elif c == 'help':
	#print('  command, option, distance')
        print('Available commands:')
        print('  help  == ')
        print('  quit')
        print('  Q:')
        print('  A:')
        #pass
    else:
        a = sendCommand(ser, c)
        print('Command sent: %s' % c)
        print('  => %s' % a)
#sys.exit(0)

#ser.write("G:".encode())
#print('M:1+P1000'.encode())
