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


## Suported Devices
Currently the following devices are supported:
- DHT - for an external connected DHT sensor (getstate and getreading + timer based updates of readings (60sec))
- LED - for the builtin LED (getstate and setstate)
- IT1, IT2, IT3 - for controlling 3 433Mhz Intertechno powerswitches (getstate and getreading + automatic state updates when a code is received)


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
