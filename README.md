# VrsTunnel
Linux NTRIP utilities

'ntclient' writes RTK correction to standard output. 
For example: 
./ntclient -a rtk.ua -p 2101 -m CMR -u myname -pw mypass -la 30 -lo 50 | /dev/ttyUSB0
