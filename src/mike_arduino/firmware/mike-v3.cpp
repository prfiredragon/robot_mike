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

/**/


#include <ros.h>
#include <Arduino.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>


#define LDir 10
#define LPWM 11
#define RDir 12
#define RPWM 6

///////////////////////////////////////////////////////////////
//Encoder pins definition

// Left encoder

#define Left_Encoder_PinA 2
#define Left_Encoder_PinB 4

int Left_Encoder_Ticks = 0;
volatile bool LeftEncoderBSet;
byte Left_Encoder_Last;

//Right Encoder

#define Right_Encoder_PinA 3
#define Right_Encoder_PinB 8
int Right_Encoder_Ticks = 0;
volatile bool RightEncoderBSet;
byte Right_Encoder_Last;



#define LOOP_DLY 10  // in msec





ros::NodeHandle nh;

std_msgs::Int16 msg_lwheel;
std_msgs::Int16 msg_rwheel;

ros::Publisher lwheel_pub("lwheel", &msg_lwheel);
ros::Publisher rwheel_pub("rwheel", &msg_rwheel);





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



ros::Subscriber<std_msgs::Float32> rmotor_sub("rmotor_cmd", &RMotorCallBack);
ros::Subscriber<std_msgs::Float32> lmotor_sub("lmotor_cmd", &LMotorCallBack);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Left_Encoder() Definitions
void do_Left_Encoder()
{
   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  Left_Encoder_Ticks += LeftEncoderBSet ? -1 : +1;
   
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
  pinMode(Left_Encoder_PinB, INPUT);      // sets pin B as input
  attachInterrupt(0, do_Left_Encoder, RISING);   //init the interrupt mode


  pinMode(Right_Encoder_PinB, INPUT);      // sets pin B as input
  attachInterrupt(1, do_Right_Encoder, RISING);   //init the interrupt mode



}




void setup(){  

  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.advertise(lwheel_pub);
  nh.advertise(rwheel_pub);

  nh.subscribe(rmotor_sub);
  nh.subscribe(lmotor_sub);


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

  lwheel_pub.publish( &msg_lwheel );
  rwheel_pub.publish( &msg_rwheel );


  
  nh.spinOnce();
  delay(LOOP_DLY);
}

