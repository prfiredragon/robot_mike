////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//// Rewrited By Roberto Rodriguez //// //// For Robot_Mike //// //// 
//// //// //// //// //// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//#include <Time.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>

#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
#include "robot_specs.h"
#include <Arduino.h>

#include <Servo.h>

#define LDir 10
#define LPWM 11
#define RDir 12
#define RPWM 6

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

///////////////////////////////////////////////////////////////
//Encoder pins definition

// Left encoder

#define Left_Encoder_PinA 2
#define Left_Encoder_PinB 4

//volatile long Left_Encoder_Ticks = 0;
volatile bool LeftEncoderBSet;
byte Left_Encoder_Last;

//Right Encoder

#define Right_Encoder_PinA 3
#define Right_Encoder_PinB 8
//volatile long Right_Encoder_Ticks = 0;
volatile bool RightEncoderBSet;
byte Right_Encoder_Last;


// Ultrasonic Sensor
#define RANGE_pin 7

// Servo
#define SERVO1 5
//Servo servo1;


#define LOOP_DLY 10  // in msec
#define LOOPTIME        100   // PID loop time(ms)

unsigned long lastMilli = 0;       // loop timing 
unsigned long lastMilliPub = 0;
double rpm_req1 = 0;
double rpm_req2 = 0;
double rpm_act1 = 0;
double rpm_act2 = 0;
double rpm_req1_smoothed = 0;
double rpm_req2_smoothed = 0;
int direction1 = FORWARD;
int direction2 = FORWARD;
int prev_direction1 = RELEASE;
int prev_direction2 = RELEASE;
int PWM_val1 = 0;
int PWM_val2 = 0;
volatile long count1 = 0;          // rev counter
volatile long count2 = 0;
long countAnt1 = 0;
long countAnt2 = 0;
float Kp =   0.5;
float Kd =   0;
float Ki =   0; //70;



ros::NodeHandle nh;

//std_msgs::Float32 msg_range;

//ros::Publisher range_pub("range", &msg_range);



//long time=0;
//int pi=3.141592654;
//int diameter=0.1524;
//int circumference=0;




void handle_cmd( const geometry_msgs::Twist& cmd_msg) {
  double x = cmd_msg.linear.x;
  double z = cmd_msg.angular.z;
  if (z == 0) {     // go straight
    // convert m/s to rpm
    rpm_req1 = x*60/(pi*wheel_diameter);
    rpm_req2 = rpm_req1;
  }
  else if (x == 0) {
    // convert rad/s to rpm
    rpm_req2 = z*track_width*60/(wheel_diameter*pi*2);
    rpm_req1 = -rpm_req2;
  }
  else {
    rpm_req1 = x*60/(pi*wheel_diameter)-z*track_width*60/(wheel_diameter*pi*2);
    rpm_req2 = x*60/(pi*wheel_diameter)+z*track_width*60/(wheel_diameter*pi*2);
  }

  rpm_req1 = constrain( rpm_req1, -MAX_RPM, MAX_RPM );
  rpm_req2 = constrain( rpm_req2, -MAX_RPM, MAX_RPM );

}


ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", handle_cmd);
geometry_msgs::Vector3Stamped rpm_msg;
ros::Publisher rpm_pub("rpm", &rpm_msg);
ros::Time current_time;
ros::Time last_time;


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

void rbrake() {


      analogWrite(RPWM, 0);
      digitalWrite(RDir, LOW);
}



//////////////////////////////////////////////////////////////////////
//                               CALLBACKS
//////////////////////////////////////////////////////////////////////


void RMotorCallBack( int motor_msg) {

    if (motor_msg > 255 || motor_msg < -255) {
    	rbrake();
    } else if (motor_msg == 0) {
    	rbrake();
    } else if (motor_msg < 0) {
    	rrev(abs(motor_msg));
    } else {
    	rfwd(motor_msg);
    }
}


void LMotorCallBack( int motor_msg) {

    if (motor_msg > 255 || motor_msg < -255) {
    	lbrake();
    } else if (motor_msg == 0) {
    	lbrake();
    } else if (motor_msg < 0) {
    	lrev(abs(motor_msg));
    } else {
    	lfwd(motor_msg);
    }
}


//////////////////////////////////////////////////////////////////////////
//void ServoCallBack( const std_msgs::Int16& servo_msg) {
//////////////////////////////////////////////////////////////////////////
//	servo1.write( constrain( servo_msg.data, 0, 179) );
//}

//ros::Subscriber<std_msgs::Int16> servo_sub("servo_cmd", &ServoCallBack);




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Left_Encoder() Definitions
void do_Left_Encoder()
{
   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  count1 += LeftEncoderBSet ? -1 : +1;
   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Right_Encoder() Definitions

void do_Right_Encoder()
{
  
  RightEncoderBSet = digitalRead(Right_Encoder_PinB);   // read the input pin
  count2 -= RightEncoderBSet ? -1 : +1;
 
 
  
}




//SetupEncoders() Definition

void SetupEncoders()
{
  // Quadrature encoders
  // Left encoder
  //pinMode(Left_Encoder_PinA, INPUT);      // sets pin A as input  
  //pinMode(Left_Encoder_PinA, INPUT);      // sets pin A as input
  pinMode(Left_Encoder_PinB, INPUT);      // sets pin B as input
  //Attaching interrupt in Left_Enc_PinA.
  //attachInterrupt(digitalPinToInterrupt(Left_Encoder_PinA), do_Left_Encoder, RISING);
  //attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(Left_Encoder_PinA),do_Left_Encoder, RISING);
  attachInterrupt(0, do_Left_Encoder, RISING);   //init the interrupt mode


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

/*

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
*/
void setup(){  

 count1 = 0;
 count2 = 0;
 countAnt1 = 0;
 countAnt2 = 0;
 rpm_req1 = 0;
 rpm_req2 = 0;
 rpm_act1 = 0;
 rpm_act2 = 0;
 PWM_val1 = 0;
 PWM_val2 = 0;

  nh.initNode();
  nh.getHardware()->setBaud(57600);
//  nh.advertise(range_pub);

//  nh.subscribe(servo_sub);
//  servo1.attach(SERVO1);

  pinMode(10,  OUTPUT);                  //direction left
  pinMode(11,  OUTPUT);                  //PWM left
  pinMode(9,  OUTPUT);                  //PWM right
  pinMode(12,  OUTPUT);                  //direction right


 nh.subscribe(sub);
 nh.advertise(rpm_pub);


   //Setup Encoders
  SetupEncoders();
}



void getMotorData(unsigned long time)  {
 rpm_act1 = double(count1-countAnt1)/double(time)*(60*1000L/double(encoder_pulse*gear_ratio));
 rpm_act2 = double(count2-countAnt2)/double(time)*(60*1000L/double(encoder_pulse*gear_ratio));//double((count2-countAnt2)*60*1000)/double(time*encoder_pulse*gear_ratio);
 countAnt1 = count1;
 countAnt2 = count2;
}

int updatePid(int id, int command, double targetValue, double currentValue, unsigned long time) {
  double pidTerm = 0;                            // PID correction
  double error = 0;
  double new_pwm = 0;
  double new_cmd = 0;
  double old_cmd = 0;
  static double last_error1 = 0;
  static double last_error2 = 0;
  static double int_error1 = 0;
  static double int_error2 = 0;
  double dt = 60*(1000L/double(time));

  error = targetValue-currentValue;
  if (id == 1) {
    int_error1 += error*dt;
    pidTerm = Kp*error + Kd*((error-last_error1)/dt) + Ki*int_error1;
    last_error1 = error;
  }
  else {
    int_error2 += error*dt;
    pidTerm = Kp*error + Kd*((error-last_error2)/dt) + Ki*int_error2;
    last_error2 = error;
  }
  //new_pwm = constrain(double(command)*MAX_RPM/4095.0L + pidTerm, -MAX_RPM, MAX_RPM);
  new_cmd = constrain(double(command)+pidTerm, -255, 255 ); //
//  old_cmd = 4095.0L*new_pwm/MAX_RPM;

  if(targetValue == 0) new_cmd = 0;
  nh.loginfo("Program info");

  char log_msg[40];


  sprintf(log_msg, "Pid: %d | RET: %d | Target: %d | Current: %d ", (int)(pidTerm), (int)(new_cmd), (int)(targetValue), int(currentValue));
  nh.loginfo(log_msg);

  return int(new_cmd);
}

void publishRPM(unsigned long time) {
  rpm_msg.header.stamp = nh.now();
  rpm_msg.vector.x = rpm_act1;
  rpm_msg.vector.y = rpm_act2;
  rpm_msg.vector.z = double(time)/1000;
  rpm_pub.publish(&rpm_msg);
  nh.spinOnce();
}

void loop(){
  nh.spinOnce();
//  msg_range.data = pingping();
//  range_pub.publish( &msg_range );


  //nh.spinOnce();
  //delay(LOOP_DLY);

  unsigned long time = millis();
  if(time-lastMilli>= LOOPTIME)   {      // enter tmed loop
    getMotorData(time-lastMilli);
    PWM_val1 = updatePid(1, PWM_val1, rpm_req1, rpm_act1, time-lastMilli);
    PWM_val2 = updatePid(2, PWM_val2, rpm_req2, rpm_act2, time-lastMilli);

    if(PWM_val1 > 0) direction1 = FORWARD;
    else if(PWM_val1 < 0) direction1 = BACKWARD;
    if (rpm_req1 == 0) direction1 = RELEASE;
    if(PWM_val2 > 0) direction2 = FORWARD;
    else if(PWM_val2 < 0) direction2 = BACKWARD;
    if (rpm_req2 == 0) direction2 = RELEASE;
    //motor1->run(direction1);
    //motor2->run(direction2);

    //motor1->setSpeed(abs(PWM_val1));
    //motor2->setSpeed(abs(PWM_val2));

    LMotorCallBack(PWM_val1);
    RMotorCallBack(PWM_val2);

    publishRPM(time-lastMilli);
    lastMilli = time;
  }
//  if(time-lastMilliPub >= LOOPTIME) {
  //  publishRPM(time-lastMilliPub);
//    lastMilliPub = time;
//  }
}

