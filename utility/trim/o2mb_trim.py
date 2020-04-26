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

PPM_STEP    = 1000
LABEL       = ""


#
# FUNCTIONS
#

def print_header( port, baud, dev_addr ):
    print( "\n" )
    print( "INFO:                   OXIGEN SENSOR" )
    print( "MODBUS:                 port:%s, baud:%d, dev#:%d" % ( port, int(baud), int(dev_addr) ) )
    print( "-----------------------------------------------------------")
    print( "+1/-1:\t\t/" )
    print( "+%d/-%d:\tPGUP/PGDN" % (PPM_STEP, PPM_STEP) )
    print( "SAVE:\t\tENTER" )
    print( "EXIT:\t\tCTRL+C" )
    print( "-----------------------------------------------------------")


def beep():
    print( '\a', end='\r' ) #make a beep

def save( ppm, mdbs ):
    dev_addr, hreg_trim = mdbs
    ppm_hi      = (ppm >> 16) & 0xFFFF
    ppm_lo      = (ppm >>  0) & 0xFFFF
    #dt          = datetime.now()
    dt          = datetime.utcnow()
    ts          = (dt - datetime(1970, 1, 1)) / timedelta(seconds=1)
    ts_hi       = ( int(ts) >> 16) & 0xFFFF
    ts_lo       = ( int(ts) >>  0) & 0xFFFF

    print( "\nTIMESTAMP: {ts}", int(ts) )
    #print( ts_hi, ts_lo )

    master.execute( dev_addr, cst.WRITE_SINGLE_REGISTER, hreg_trim+0, output_value=ppm_hi)
    master.execute( dev_addr, cst.WRITE_SINGLE_REGISTER, hreg_trim+1, output_value=ppm_lo)
    master.execute( dev_addr, cst.WRITE_SINGLE_REGISTER, hreg_trim+2, output_value=ts_hi)
    master.execute( dev_addr, cst.WRITE_SINGLE_REGISTER, hreg_trim+3, output_value=ts_lo)

    beep()


def print_ppm( ppm ):
    print( f'{ppm:10d} ppm / {(ppm/10000):5.2f}%\t', end='\r' )


def chck_key( ppm, mdbs ):
    try:
        c = click.getchar()
        if( c == '\r' ):
            save( ppm, mdbs )
        elif( c[1] == 'P' ):
            if(ppm > 0):
                ppm -= 1
                print_ppm( ppm )
            else:
                beep()
        elif( c[1] == 'H' ):
            if(ppm < 2**32):
                ppm += 1
                print_ppm( ppm )
            else:
                beep()
        elif( c[1]=='Q' ):
            if(ppm >= PPM_STEP):
                ppm -= PPM_STEP
                print_ppm( ppm )
            else:
                beep()
        elif( c[1]=='I' ):
            if(ppm < (2**32-PPM_STEP)):
                ppm += PPM_STEP
                print_ppm( ppm )
            else:
                beep()
    except KeyboardInterrupt:
        print( "\n\r")
        raise SystemExit(0)
    return( ppm )


def main( mdbs ):
    dev_addr, hreg_trim = mdbs
    m   = master.execute( dev_addr, cst.READ_HOLDING_REGISTERS, hreg_trim, 8 )
    ppm = (m[ 0] << 16) + (m[ 1] & 0xFFFF)
    t   = (m[ 2] << 17) + (m[ 3] & 0xFFFF)
    print_ppm( ppm )
    while True:
        ppm = chck_key( ppm, mdbs )

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
