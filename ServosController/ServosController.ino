#include <AX12A.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#define DirectionPin (5u)
#define BaudRate (1000000ul)

#define UA      (8)     // upperarm servo
#define BA      (61)    // bottomarm servo
#define Magnet  (22)    // magnet servo
#define Hopper  (13)    // hopper servo
#define test    (1)     // tests ervo
#define arm     (69)    // geen idee
#define wheel1  (61)    // left wheel servo
#define wheel2  (10)    // right wheel servo

String msg;
SoftwareSerial softwareSerial(15, 16);

// middle point of servo in degrees and decimal
int initial_pos = 512;
int initial_deg = 150;

// initialisation for reading inputs
String servo;
int id;
int graden;
String command;

// standard speed value
int speed = 200;

// start setup
void setup()
{
  softwareSerial.begin(9600);
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  // ax12a.setEndless(1, OFF);
}


// start loop
void loop()
{
  readSerialPort();

  
  if (msg != "")
  {

    splitString();
    
    softwareSerial.print("hey ");

 
// Initialise the right id based on the string that was given
    if (servo == "hopper"){ id = 13; softwareSerial.print(" hopper servo"); }
    else if (servo == "magnet"){ id = 22; softwareSerial.print(" magnet servo"); }
    else if (servo == "test"){ id = 1; softwareSerial.print(" test servo  "); }
    else if (servo == "bottomarm"){ id = 8; softwareSerial.print(" bottomarm servo  "); }
    else if (servo == "upperarm"){ id = 69; softwareSerial.print(" upperarm servo  "); }
    else if (servo == "wheel1"){ id = 61; softwareSerial.print(" left wheel servo  "); }
    else if (servo == "wheel2"){ id = 10; softwareSerial.print(" right wheel servo  "); }
    else if (servo == "back wheel"){ id = 144; softwareSerial.print(" back wheel servo  "); }


// If there are degrees given move servo to the given degrees   
    if (graden != NULL)
    {
// servo is not allowed to turn further than than 30 deg both ways
      if (servo == "magnet")
      {
        if (graden > 30 || graden < -30)
        {
          return;
        }
      ax12a.move(id, convertDegrees(graden));
    }
    else if (servo == "hopper")
    {
      // make spin
      ax12a.setEndless(id, ON);
      ax12a.turn(id, LEFT, speed);
    }
    else if (graden == 0) {
      ax12a.move(id, 512);
    }
    else{
      softwareSerial.print("helpwhatareyoudoing");
    }
    delay(500);
  }
}


// Read messages that come in through the serial port
  void readSerialPort()
  {
    msg = "";
    if (softwareSerial.available())
    {
      delay(10);
      while (softwareSerial.available() > 0)
      {
        msg += (char)softwareSerial.read();
      }
      softwareSerial.flush();
    }
  }

// The servo is only able to turn 300 degrees, the middle is 150 degrees
// The function calculates the decimal it has to turn from the middle based on the degrees given
  int convertDegrees(int extra_deg)
  {
    long wanted_deg = initial_deg + extra_deg;
    int result = floor((wanted_deg * initial_pos) / initial_deg);

    return result;
  }

// gets the input string and splits it with the ; as a seperator
  void splitString()
  {
    int index = msg.indexOf(';');
    servo = msg.substring(0, index);
    String input2 = msg.substring(index + 1, msg.length());
    
    if (servo == "hopper"){ speed = input2.toInt(); }
    else { graden = input2.toInt();  }
  }
  