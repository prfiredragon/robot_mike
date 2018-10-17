//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                    Rewrited By Roberto Rodriguez                                                             ////
////                    For Robot_Mike                                                                            ////
////                                                                                                              ////
////                                                                                                              ////
////                                                                                                              ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//#include <Time.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
//Make a pin into an interrupt
//#include <PinChangeInterrupt.h>
#include <Arduino.h>

#include <Servo.h>

#define LDir 10
#define LPWM 11
#define RDir 12
#define RPWM 6

///////////////////////////////////////////////////////////////
//Encoder pins definition

// Left encoder

#define Left_Encoder_PinA 2
#define Left_Encoder_PinB 4

volatile int Left_Encoder_Ticks = 0;
volatile bool LeftEncoderBSet;
volatile byte Left_Encoder_Last;
volatile boolean LDirection;

//Right Encoder

#define Right_Encoder_PinA 3
#define Right_Encoder_PinB 8
volatile int Right_Encoder_Ticks = 0;
volatile bool RightEncoderBSet;
volatile byte Right_Encoder_Last;
volatile boolean RDirection;

// Ultrasonic Sensor
#define RANGE_pin 7

// Servo
#define SERVO1 5
Servo servo1;


#define LOOP_DLY 10  // in msec





ros::NodeHandle nh;

std_msgs::Int16 msg_lwheel;
std_msgs::Int16 msg_rwheel;
std_msgs::Float32 msg_range;

ros::Publisher lwheel_pub("lwheel", &msg_lwheel);
ros::Publisher rwheel_pub("rwheel", &msg_rwheel);
ros::Publisher range_pub("range", &msg_range);



long time=0;
//int pi=3.141592654;
//int diameter=0.1524;
//int circumference=0;




//////////////////////////////////////////////////////////////////////
//                               LEFT
//////////////////////////////////////////////////////////////////////

void lfwd(int speed=255) {



      analogWrite(LPWM, constrain( speed, 0, 255 ));
      digitalWrite(LDir, LOW);

}


void lrev(int speed=255) {


      analogWrite(LPWM, constrain(speed, 0, 255 ));
      digitalWrite(LDir, HIGH);

}

void lcoast() {


      analogWrite(LPWM, 0);
      digitalWrite(LDir, HIGH);
}

void lbrake() {


      analogWrite(LPWM, 0);
      digitalWrite(LDir, LOW);
}



//////////////////////////////////////////////////////////////////////
//                               RIGHT
//////////////////////////////////////////////////////////////////////

void rfwd( int speed=255) {


      analogWrite(RPWM, constrain(speed, 0, 255 ));
      digitalWrite(RDir, LOW);

}

void rrev(int speed=255) {


      analogWrite(RPWM, constrain(speed, 0, 255 ));
      digitalWrite(RDir, HIGH);

}


void rcoast() {


      analogWrite(RPWM, 0);
      digitalWrite(RDir, HIGH);
}

void rbrake() {


      analogWrite(RPWM, 0);
      digitalWrite(RDir, LOW);
}



//////////////////////////////////////////////////////////////////////
//                               CALLBACKS
//////////////////////////////////////////////////////////////////////


void RMotorCallBack( const std_msgs::Float32& motor_msg) {

    if (motor_msg.data > 255 || motor_msg.data < -255) {
    	rbrake();
    } else if (motor_msg.data == 0) {
    	rcoast();
    } else if (motor_msg.data < 0) {
    	rrev(abs(motor_msg.data));
    } else {
    	rfwd(motor_msg.data);
    }
}


void LMotorCallBack( const std_msgs::Float32& motor_msg) {

    if (motor_msg.data > 255 || motor_msg.data < -255) {
    	lbrake();
    } else if (motor_msg.data == 0) {
    	lcoast();
    } else if (motor_msg.data < 0) {
    	lrev(abs(motor_msg.data));
    } else {
    	lfwd(motor_msg.data);
    }
}


//////////////////////////////////////////////////////////////////////////
//void ServoCallBack( const std_msgs::Int16& servo_msg) {
//////////////////////////////////////////////////////////////////////////
//	servo1.write( constrain( servo_msg.data, 0, 179) );
//}


ros::Subscriber<std_msgs::Float32> rmotor_sub("rmotor_cmd", &RMotorCallBack);
ros::Subscriber<std_msgs::Float32> lmotor_sub("lmotor_cmd", &LMotorCallBack);

//ros::Subscriber<std_msgs::Int16> servo_sub("servo_cmd", &ServoCallBack);




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Left_Encoder() Definitions
void do_Left_Encoder()
{
int Lstate = digitalRead(Left_Encoder_PinA);
  if((Left_Encoder_Last == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(Left_Encoder_PinB);
    if(val == LOW && LDirection)
    {
      LDirection = false; //Reverse
    }
    else if(val == HIGH && !LDirection)
    {
      LDirection = true;  //Forward
    }
  }
  Left_Encoder_Last = Lstate;
  
  if(!LDirection)  Left_Encoder_Ticks++;
  else  Left_Encoder_Ticks--;


   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  //LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  //Left_Encoder_Ticks += LeftEncoderBSet ? -1 : +1;
   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Right_Encoder() Definitions

void do_Right_Encoder()
{
  
  RightEncoderBSet = digitalRead(Right_Encoder_PinB);   // read the input pin
  Right_Encoder_Ticks -= RightEncoderBSet ? -1 : +1;
 
 
  
}




//SetupEncoders() Definition

void SetupEncoders()
{
  LDirection = true;
  // Quadrature encoders
  // Left encoder
  //pinMode(Left_Encoder_PinA, INPUT);      // sets pin A as input  
  //pinMode(Left_Encoder_PinA, INPUT);      // sets pin A as input
  pinMode(Left_Encoder_PinB, INPUT);      // sets pin B as input
  //Attaching interrupt in Left_Enc_PinA.
  //attachInterrupt(digitalPinToInterrupt(Left_Encoder_PinA), do_Left_Encoder, RISING);
  //attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(Left_Encoder_PinA),do_Left_Encoder, RISING);
  attachInterrupt(0, do_Left_Encoder, CHANGE);   //init the interrupt mode


  // Right encoder
  //pinMode(Right_Encoder_PinA, INPUT);      // sets pin A as input
  //pinMode(Right_Encoder_PinA, INPUT);      // sets pin A as input
  pinMode(Right_Encoder_PinB, INPUT);      // sets pin B as input
  //Attaching interrupt in Right_Enc_PinA.
  //attachInterrupt(digitalPinToInterrupt(Right_Encoder_PinA), do_Right_Encoder, RISING); 
  //attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(Right_Encoder_PinA),do_Right_Encoder, RISING);
  attachInterrupt(1, do_Right_Encoder, RISING);   //init the interrupt mode

  //attachInterrupt(0,wheelSpeed, CHANGE);

}



float microsecondsToCentimeters(float microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

float cmToMeters(float cm){
return cm / 100;
}

float pingping(){
  float duration, cm, meters;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(RANGE_pin, OUTPUT);
  digitalWrite(RANGE_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(RANGE_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(RANGE_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(RANGE_pin, INPUT);
  duration = pulseIn(RANGE_pin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  meters = cmToMeters(cm);
return meters;
}

//////////////////////////////////////////////////////////////////////////
void ServoCallBack( const std_msgs::Int16& servo_msg) {
//////////////////////////////////////////////////////////////////////////
        servo1.write( constrain( servo_msg.data, 0, 179) );
  msg_range.data = pingping();
  range_pub.publish( &msg_range );

}

ros::Subscriber<std_msgs::Int16> servo_sub("servo_cmd", &ServoCallBack);

void setup(){  

  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.advertise(lwheel_pub);
  nh.advertise(rwheel_pub);
  nh.advertise(range_pub);
  nh.subscribe(rmotor_sub);
  nh.subscribe(lmotor_sub);

  nh.subscribe(servo_sub);
  servo1.attach(SERVO1);

  pinMode(10,  OUTPUT);                  //direction left
  pinMode(11,  OUTPUT);                  //PWM left
  pinMode(9,  OUTPUT);                  //PWM right
  pinMode(12,  OUTPUT);                  //direction right



   //Setup Encoders
  SetupEncoders();
}

void loop(){
  nh.spinOnce();
  msg_lwheel.data = Left_Encoder_Ticks;
  msg_rwheel.data = Right_Encoder_Ticks;
//  msg_range.data = pingping();
  lwheel_pub.publish( &msg_lwheel );
  rwheel_pub.publish( &msg_rwheel );
//  range_pub.publish( &msg_range );

  
  nh.spinOnce();
  delay(LOOP_DLY);
}

