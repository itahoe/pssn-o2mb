@echo off
set PORT="COM4"
set BAUD=19200
set DEV_ADDR=13
set HREG_TRIM=37
set PPM=90000

o2mb_offset.py %PORT% %BAUD% %DEV_ADDR% %HREG_TRIM% %PPM%
