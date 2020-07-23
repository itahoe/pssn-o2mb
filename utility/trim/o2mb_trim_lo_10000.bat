@echo off
set PORT="COM4"
set BAUD=19200
set DEV_ADDR=13
set HREG_TRIM=48
set PPM=10000

o2mb_trim.py %PORT% %BAUD% %DEV_ADDR% %HREG_TRIM% %PPM%
