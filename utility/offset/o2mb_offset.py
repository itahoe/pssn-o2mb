import sys
import serial
import modbus_tk.defines as cst
import modbus_tk.modbus_rtu as modbus_rtu
import click
from datetime import timedelta
from datetime import datetime
from threading import Timer
from datetime import timezone


#
# CONFIGURATION
#

#PORT        = "COM4"
#BAUD        = 19200
#DEV_ADDR    = 13
#REG_ADDR    = 0
#TRIM_ADDR   = 52

PPM_STEP    = 1000
LABEL       = ""


#
# FUNCTIONS
#

def print_header( port, baud, dev_addr ):
    print( "\n" )
    print( "INFO:                   OXIGEN SENSOR - SET INPUT OFFSET" )
    print( "MODBUS:                 port:%s, baud:%d, dev#:%d" % ( port, int(baud), int(dev_addr) ) )
    print( "-----------------------------------------------------------")
    print( "+1/-1:\t\t/" )
    print( "+%d/-%d:\tPGUP/PGDN" % (PPM_STEP, PPM_STEP) )
    print( "SAVE:\t\tENTER" )
    print( "EXIT:\t\tCTRL+C" )
    print( "-----------------------------------------------------------")


def beep():
    print( '\a', end='\r' ) #make a beep


def save( offset_raw, mdbs ):
    dev_addr, hreg_trim = mdbs
    master.execute( dev_addr, cst.WRITE_SINGLE_REGISTER, hreg_trim+0, output_value=offset_raw)
    beep()


def print_ppm( ppm ):
    #print( "%10d ppm / %5.2f%%\t" % (ppm , ppm/10000), end='\r' )
    #print( f'{ppm:10d} ppm / {(ppm/10000):5.2f} %%\t', end='\r' )
    print( f'OFFSET: {ppm:6d}\t', end='\r' )


def chck_key( val, mdbs ):
    try:
        c = click.getchar()
        if( c == '\r' ):
            save( val, mdbs )
        elif( c[1] == 'P' ):
            if(val > 0):
                val -= 1
                print_ppm( val )
            else:
                beep()
        elif( c[1] == 'H' ):
            if(val < (2**16)-1):
                val += 1
                print_ppm( val )
            else:
                beep()
        elif( c[1]=='Q' ):
            if(val >= PPM_STEP):
                val -= PPM_STEP
                print_ppm( val )
            else:
                beep()
        elif( c[1]=='I' ):
            if(val < (2**16-PPM_STEP)):
                val += PPM_STEP
                print_ppm( val )
            else:
                beep()
    except KeyboardInterrupt:
        print( "\n\r")
        raise SystemExit(0)
    return( val )
'''

def print_sens( m ):
    ppm         = (m[ 0] << 16) + (m[ 1] & 0xFFFF)
    print( "sens", ppm, end='\r' )
'''

#def main( dev_addr, hreg_trim ):
def main( mdbs ):
    #print_header()
    dev_addr, hreg_trim = mdbs
    m   = master.execute( dev_addr, cst.READ_HOLDING_REGISTERS, hreg_trim, 8 )
    val = m[ 0]
    print_ppm( val )
    while True:
        val = chck_key( val, mdbs )

#
# START EXECUTION
#

if __name__ == "__main__":
    try:
        port        =   sys.argv[1]
        baud        =   sys.argv[2]
        dev_addr    =   sys.argv[3]
        hreg_trim   =   sys.argv[4]
    except:
        print( "\n\rUSAGE: trim_o2mb.py <ser_port> <baudrate> <dev_address> <hreg_trim>" )
        sys.exit( 1 )

    mdbs        = int(dev_addr), int(hreg_trim)
    master      = modbus_rtu.RtuMaster( serial.Serial(port=port, baudrate=baud, bytesize=8, parity='N', stopbits=1, xonxoff=0) )
    master.set_timeout( 2.0 )
    master.set_verbose( False )
    print_header( port, baud, dev_addr )
    main( mdbs )
