#import modbus_tk.defines as cst
#import modbus_tk.modbus_rtu as modbus_rtu
#import serial
from serial.tools import list_ports



import serial.tools.list_ports

def main():
    try:
        #print( help( 'serial.tools.list_ports' ) )
        #print( help( 'serial.tools' ) )
        #print( serial.tools.list_ports )

        x = list_ports.comports()
        print( x )
        print( list(x) )



        #plist = serial.tools.list_ports.comports()
        #print(plist)
        #print( list(plist) )

    except KeyboardInterrupt:
        print( "\n\r")
        raise SystemExit(0)


#
# START EXECUTION
#
if __name__ == "__main__":
    main()
