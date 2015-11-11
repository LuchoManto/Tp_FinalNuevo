__author__ = 'Gaston'

import serial

# Configuramos el puerto serial.
serial = serial.Serial(port="COM4",
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

def main():
    print "Iniciado"
    while 1:
        command = serial.readline()
        print 'Recieved from NODEMcu: ' + command

        if command == 'respuesta\n':
            enviar = '1'
            serial.write(enviar)
            print 'Send to Node: ' + enviar

        if command == '04\n':
            enviar = 'hola como te va'
            serial.write(enviar.encode("hex"))
            print 'Send to Node: ' + str(enviar.encode("hex"))

    return


if __name__ == "__main__":
    main()