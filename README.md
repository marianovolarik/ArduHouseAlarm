# ArduHouseAlarm
A simple house alarm implemented on Arduino



## HouseAlarm ##

### States ###
It has the following states:

![FSM](https://github.com/marianovolarik/ArduHouseAlarm/blob/master/docs/states_graphic.png)

+ **PRE_ACTIVATED**: The time period until the alarm is activated. This time let you leave the house. 
  If it is not deactivated, after `PRE_ACTIVATED_TIME_S` seconds, it will activate.
+ **ACTIVATED**: The alarm is sensing the sensors.
+ **PRE_RINGING**: The time period of `PRE_RINGING_TIME_S` seconds after the alarm will ring.
+ **RINGING**: The alarm is ringing.
+ **DEACTIVATED** The alarm is deactivated.   


### Methods ###

+ Constructor `HouseAlarm(int BuzzerPin)` takes the pin which a buzzer is connected.
+ `begin()` initializes the system.
+ `run()` checks the integrity of the system.
+ `activate()` actives the alarm.
+ `ring()` if the alarm is activated, `ring()` changes the state to PRE_RINGING.
+ `is_ringing()` returns `true` if and only if state of the alarm is RINGING.
+ `deactivate(char *pwd)` deactivates the alarm if `pwd` matches with `DEACTIVATION_CODE`.








