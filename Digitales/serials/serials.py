__author__ = 'Gaston'

import serial
import serial.tools.list_ports
import time
import threading

from helpers.config import *

Serialport = serial.Serial()

# port="COM5",
#         baudrate=9600,
#         parity=serial.PARITY_ODD,
#         stopbits=serial.STOPBITS_ONE,
#         bytesize=serial.EIGHTBITS

def read_serial(logger=None):
    try:
        while True:
            serial_data = Serialport.readline()
            serial_data = serial_data.split()[0]
            logger.info('Respuesta: ' + str(serial_data))
            log_response(str(serial_data))
    except Exception:
        pass


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
        t = threading.Thread(target=read_serial, args=[logger])
        t.start()

    except Exception:
        logger.info("Ocurrio un problema al abrir " + puerto)
        pass




def send_serial(dato, logger = False):
    try:
        Serialport.write(dato)
    except Exception:
        pass
    return


def get_com():
    puertos = []
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        puertos.append(p[0])

    return puertos