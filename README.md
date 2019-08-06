# VrsTunnel
Linux NTRIP utilities

'ntclient' writes RTK correction to standard output. 
For example: 

./ntclient -a rtk.ua -p 2101 -m CMR -u myname -pw mypass -la 51 -lo 32 > /dev/ttyUSB0

./ntclient -a vrs.ua -p 2102 -m RTCM3 -u myname -pw mypass -la 51 -lo 32 | pv -ptebar > /dev/null

NMEA input is not supported by 'ntclient' currently
