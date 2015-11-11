__author__ = 'Gaston'

import serial

Serialport = serial.Serial()

# port="COM5",
#         baudrate=9600,
#         parity=serial.PARITY_ODD,
#         stopbits=serial.STOPBITS_ONE,
#         bytesize=serial.EIGHTBITS

def connect():
    Serialport.port = 'COM4'
    Serialport.baudrate = 9600
    Serialport.parity = serial.PARITY_ODD
    Serialport.stopbits = serial.STOPBITS_ONE
    Serialport.bytesize = serial.EIGHTBITS
    Serialport.open()


def read_serial():
    serial_data = Serialport.readline()
    return serial_data

def send_serial(dato):
    connect()
    Serialport.write(dato)
    return
