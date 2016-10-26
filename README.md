# Aquarius

Aquarius is the firmware for an aquarium controller that manages all necessary sensors and actors in and around an aquarium.

Supported Sensors:
- DHT22 for measuring air temperature and humidity
- DS18B20 for measuring water temperature
- Light sensor

Supported actors
- 4 channel LED driver with PWM dimming support
- 433MHz Transmitter for controller switch outlets
- 4 channels for controlling pumps


## HTTP Integration into FHEM

## Serial Command Line Interface
### Command Format
```
getstate <DEVICE>
getreading <DEVICE>

setstate <DEVICE> <STATE>
```

Some examples
```
getstate IT 123123 0
--> STATE IT 123123 0 OFF
```

```
setstate IT1 ON
--> STATE IT1 ON
```

```
getstate DHT
--> STATE DHT T: 21.2 H: 55.3
```

```
getreading DHT
--> READING DHT Temperature 21.2
--> READING DHT Humidity 55.3
```
