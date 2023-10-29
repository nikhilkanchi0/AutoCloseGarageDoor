# AutoCloseGarageDoor
 Senses if the garage door is open or closed using an Ultosonic Sensor. 
 If the garage door has been open for 10 minutes or longer then trigger a 
 relay module to close it.
 
 For details on the sensor, 
 see: https://docs.arduino.cc/built-in-examples/sensors/Ping

 For details on the Relay Module, 
 see: https://www.aranacorp.com/en/using-a-multi-channel-relay-module-with-arduino/

 For details on the LCD Display, 
 see: https://docs.arduino.cc/learn/electronics/lcd-displays

 Uses a relay module to interface and toggle the garage door remote control switch.

 Circuit:
 Ultrasonic sensors attached to pins as follows:
 Sensor1: (Trigg: Pin 3, Echo: Pin 4) 
 Sensor2: (Trigg: Pin 6, Echo: Pin 7)

 Relay Module attached to pins as follows:
 Relay1: Pin 2
 Relay2: Pin 5

 LCD Display: To show the current state (Open or Close) of the garage doors and 
 if open how long it has been open for. 
 RegisterSlect (rs): Pin 8
 Enable Pin (en): Pin 9
 Data (d4): Pin 10
 Data (d5): Pin 11
 Data (d6): Pin 12
 Data (d4): Pin 13

 When to close the door: If the garage door has been open for 10 minutes or longer 
 then trigger the relay module to close it.

 created by Nikhil Kanchi
