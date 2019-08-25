## Linux NTRIP utilities

'ntclient' writes RTK correction to standard output. 
For example: 

./ntclient -a rtk.ua -p 2101 -m CMR -u myname -pw mypass -la 51 -lo 32 > /dev/ttyUSB0

./ntclient -a vrs.ua -p 2102 -m RTCM3 -u myname -pw mypass -la 51.123 -lo 32.654 | pv -ptebar > /dev/null

NMEA input is not supported by 'ntclient' currently.

One NTRIP Caster can be bridged to another one by piping ntclient to ntserver. For example:

./ntclient -a src.sm -p 2101 -m CMR -u snm -pw sps -la 51.12 -lo 32.45 | ./ntserver -a dest.sm -p 2103 -m pCMR -u pnm -pw pwd -la 51.12 -lo 32.45

Detailed description of the source code is published here
https://pvvovan.github.io/VrsTunnel/index.html
