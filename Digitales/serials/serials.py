__author__ = 'Gaston'

import serial
import serial.tools.list_ports

Serialport = serial.Serial()

# port="COM5",
#         baudrate=9600,
#         parity=serial.PARITY_ODD,
#         stopbits=serial.STOPBITS_ONE,
#         bytesize=serial.EIGHTBITS


def connect(logger, puerto='COM4'):
    try:
        Serialport.close()
    except Exception:
        pass

    try:
        logger.info("Trantando de abrir puerto " + puerto)
        Serialport.port = puerto
        Serialport.baudrate = 9600
        Serialport.parity = serial.PARITY_ODD
        Serialport.stopbits = serial.STOPBITS_ONE
        Serialport.bytesize = serial.EIGHTBITS
        Serialport.open()
        logger.info("Puerto " + puerto + " abierto correctamente")

    except Exception:
        logger.info("Ocurrio un problema al abrir " + puerto)
        pass

def read_serial(logger = False):
    #serial_data = Serialport.readline()
    dato = False
    veces = 0
    serial_data = ''
    while dato is False:
        if(Serialport.inWaiting() > 0):
            serial_data += Serialport.read()
        else:
            veces += 1
        if(veces == 100):
            dato = True

    return serial_data

def send_serial(dato, logger = False):
    Serialport.write(dato)
    return

def get_com():
    puertos = []
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        puertos.append(p[0])

    return puertos