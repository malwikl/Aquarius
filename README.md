# Aquarius

## Command Format
'''
getstate <DEVICE>
getreading <DEVICE>

setstate <DEVICE> <STATE>
'''

e.g.
'''
getstate IT 123123 0
--> STATE IT 123123 0 OFF
'''

setstate IT1 ON
--> STATE IT1 ON

getstate DHT
--> STATE DHT temp:23,2 hum:55
