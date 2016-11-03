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
- TEMP1 - for reading temperature from a DS18B20

## Supported Readings
- temperature (for TEMP1 and DHT)
- humidity (for DHT)
- state (for all types)

## Serial Command Line Interface
### Command Format
```
getreading <DEVICE> <READING>

setstate <DEVICE> <STATE>
```

Some examples
```
getreading IT1 state
--> READING IT1 state OFF
```

```
setstate IT1 ON
--> STATE IT1 ON
```

```
getreading DHT state
--> READING DHT state T: 21.2 H: 55.3
```

```
getreading DHT temperature
--> READING DHT temperature 21.2
getreading DHT humidity
--> READING DHT humidity 55.3
```
