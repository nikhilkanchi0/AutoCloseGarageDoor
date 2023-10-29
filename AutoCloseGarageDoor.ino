/*
 Auto Close Garage Door

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
 */
 
 #include <LiquidCrystal.h>

const bool DEBUG = false;

bool toggle = true;
const int garageOpenerPins[2] = { 2, 5 };
const int doorSensorTriggerPins[2] = { 3, 6 };
const int doorSensorEchoPins[2] = { 4, 7 };
const int maxOpenDuration = 600;  //In seconds

int numDoors = 2;
unsigned long garageDoorClosedEndTime[2] = { 0, 0 };

//LCD Pannel Control Pins
const int rs = 8,
          en = 9,
          d4 = 10,
          d5 = 11,
          d6 = 12,
          d7 = 13;
char buffer[100];
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String formatTime(int seconds) {
  int min = seconds / 60;
  int sec = seconds % 60;
  char mmss[10];

  sprintf(mmss, "%02d:%02d", min, sec);
  return (mmss);
}


void screenMessage() {
  lcd.begin(16, 2);
  lcd.blink();
  lcd.setCursor(0, 0);
  lcd.print("AutoClose Garage");
  lcd.setCursor(0, 1);
  lcd.print("by Nikhil Kanchi");
  delay(2000);
  lcd.clear();
  lcd.noBlink();
}

int getDoorDistance(int doorNum) {
  long duration;
  int distance;

  digitalWrite(doorSensorTriggerPins[doorNum], LOW);
  delayMicroseconds(2);
  digitalWrite(doorSensorTriggerPins[doorNum], HIGH);
  delayMicroseconds(10);
  digitalWrite(doorSensorTriggerPins[doorNum], LOW);

  duration = pulseIn(doorSensorEchoPins[doorNum], HIGH);
  
  //Calculate distance in centimeter based on duration of the pulse
  distance = duration * 0.034 / 2;

  if (DEBUG) {
    sprintf(buffer, "Distance of Door# %d : %d cm", doorNum + 1, distance);
    Serial.println(buffer);
  }
  return (distance);
}

void closeDoor(int doorNum) {
  digitalWrite(garageOpenerPins[doorNum], LOW);
  delay(500);
  digitalWrite(garageOpenerPins[doorNum], HIGH);
  return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  unsigned long currentTime = millis();
  for (int i = 0; i < numDoors; i++) {
    // Set the OpenerPins to Output and Closed state)
    pinMode(garageOpenerPins[i], OUTPUT);
    digitalWrite(garageOpenerPins[i], HIGH);

    //Set the trigger and echo pins for I/O
    pinMode(doorSensorTriggerPins[i], OUTPUT);
    pinMode(doorSensorEchoPins[i], INPUT);

    // Initialize the grage door open duration to 0
    garageDoorClosedEndTime[i] = currentTime;
  }
  screenMessage();
}

void loop() {
  // put your main code here, to run repeatedly:
  String strDisplay;
  unsigned long currentTime = millis();
  for (int i = 0; i < numDoors; i++) {
    String doorNum = String(i + 1);
    lcd.setCursor(0, i);
    String strPrint = String("Door" + doorNum + ":");
    int distance = getDoorDistance(i);
    int durationOpen = 0;    
    if (distance < 50) { 
      // Distance from the sensor to the door < 50cm means door is open
      // calculate the duration it has been open in seconds
      // CurrentTime - ClosedEndTime
      durationOpen = (currentTime - garageDoorClosedEndTime[i])/1000;
      // Check if garage door has been open for more than maxAllowed duration
      if (durationOpen > maxOpenDuration) {
        // Attempt to close the door
        strPrint = strPrint + "Closing   ";
        closeDoor(i);
        garageDoorClosedEndTime[i] = currentTime;
      } else {
        if ( toggle )
          strDisplay = formatTime(durationOpen);
        else 
          strDisplay = String(String(distance) + "cm  ");
        strPrint = strPrint + String("Open " + strDisplay + "     ");
      }
    } else {
      // Garage door is closed, so set the duration it has been open to 0
      garageDoorClosedEndTime[i] = currentTime;
      strPrint = strPrint + "Closed    ";
    }
    if (DEBUG) 
      Serial.println(strPrint);
    lcd.print(strPrint);
    //delay(100);
  }
  toggle = !toggle;
  delay(1000);
}
