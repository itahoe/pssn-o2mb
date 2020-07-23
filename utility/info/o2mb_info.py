import serial
import modbus_tk.defines as cst
import modbus_tk.modbus_rtu as modbus_rtu
import click
from datetime import datetime
from datetime import date
from time import sleep
from struct import pack, unpack, unpack_from
import ctypes

#
# CONFIGURATION
#

PORT        = "COM4"
BAUD        = 19200
DEV_ADDR    = 13
REG_ADDR    = 0


#
# FUNCTIONS
#

def print_header():
    print( "\n" )
    print( "INFO:                   OXIGEN SENSOR" )
    print( "MODBUS:                 port:%s, baud:%d, dev#:%d" % ( PORT, BAUD, DEV_ADDR ) )


def print_id( dev, hw, fw ):
    #print( f'device_id:\t\t{dev[0]:04X}{dev[1]:04X}' )
    #print( f'device_id:\t\t{" ".join(str(x) for x in dev[0:2])}' )
    #print( f'device_id:\t\t{(int(x).to_bytes for x in dev[0:2])}' )
    #print( f"device_id:\t\t {dev[0].to_bytes(2,byteorder='big') }" )
    d = list( dev[0].to_bytes(2,byteorder='little') + dev[1].to_bytes(2,byteorder='little') )
    print( f"device_id:\t\t{d[0]:02X}{d[1]:02X}{d[2]:02X}{d[3]:02X}" )
    print( f'hardware_id:\t\t{hw[0]:04X}{hw[1]:04X}' )
    print( f'firmware_id:\t\t{fw[0]:04x}{fw[1]:04X}{fw[3]:04X}' )


def print_serial_num( m ):
    print( "serial#:\t\t%04X%04X %04X%04X %04X%04X %04X%04X" % (m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7]) )


def print_starts_count( m ):
    print( "starts_counter:         %d" % ( m[ 0] ) )


def print_trim( m ):
    ppm = (m[ 0] << 16) + (m[ 1] & 0xFFFF)
    ts  = (m[ 2] << 16) + (m[ 3] & 0xFFFF)
    print( "ppm: %10d\t" % ppm, "timestamp:", datetime.utcfromtimestamp( ts ) )


def main():
    master      = modbus_rtu.RtuMaster( serial.Serial(port=PORT, baudrate=BAUD, bytesize=8, parity='N', stopbits=1, xonxoff=0) )
    master.set_timeout( 2.0 )
    master.set_verbose( False )
    m           = master.execute( DEV_ADDR, cst.READ_HOLDING_REGISTERS, REG_ADDR, 64 )
    #print( m )
    print_header()
    print( "-----------------------------------------------------------")
    print_id( m[ 0:], m[ 2:], m[ 4:] )
    #print_hardware_id(  m[ 2:] )
    #print_firmware_id(  m[ 8:] )
    #print_version(      m[ 2:], m[ 4:] )
    print_serial_num(   m[16:] )
    print_starts_count( m[ 7:] )
    print_trim(         m[48:] )
    print_trim(         m[52:] )
    print( "-----------------------------------------------------------")
    print( "  RAW\t\tPPM\tt_BRD\tt_MCU\tvdda_MCU INSTABILITY  ")

    while True:
        try:
            m           = master.execute( DEV_ADDR, cst.READ_HOLDING_REGISTERS, 0, 64 )

            ppm         = (m[33] << 16) + (m[32] & 0xFFFF)
            raw         = m[34]
            instability = m[35]
            brd_temp    = m[36]
            mcu_temp    = m[40]
            mcu_vdda    = m[41]

            if( int(ppm) & 0x80000000):
                ppm = -0x100000000 + int(ppm)

            print( f'{raw:5d}\t{ppm:10d}\t{brd_temp:3d}\xB0C\t{mcu_temp:3d}\xB0C\t{mcu_vdda/1000:3.2f}V {instability:10d}', end='\r' )
            sleep(1.0)
            #n  = m[0]
            #print( n.to_bytes(4, byteorder='big', signed=True) )

        except KeyboardInterrupt:
            print( "\n\r")
            raise SystemExit(0)


#
# START EXECUTION
#
if __name__ == "__main__":
    main()