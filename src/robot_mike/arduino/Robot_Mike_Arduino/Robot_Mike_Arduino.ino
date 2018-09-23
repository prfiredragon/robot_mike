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

#define LDir 10
#define LPWM 11
#define RDir 12
#define RPWM 9


ros::NodeHandle nh;

std_msgs::String msg_debug;
ros::Publisher debug_pub("arduino_debug", &msg_debug);
std_msgs::Int16 msg_lwheel;
std_msgs::Int16 msg_rwheel;

ros::Publisher lwheel_pub("lwheel", &msg_lwheel);
ros::Publisher rwheel_pub("rwheel", &msg_rwheel);

char debug_str[80] = "blank";


long time=0;
//int pi=3.141592654;
//int diameter=0.1524;
//int circumference=0;




//////////////////////////////////////////////////////////////////////
//                               LEFT
//////////////////////////////////////////////////////////////////////

void lfwd(int speed=255) {
	sprintf(debug_str, "lfwd %d", speed);
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );



      analogWrite(LPWM, constrain( speed, 0, 255 ));
      digitalWrite(LDir, LOW);

}


void lrev(int speed=255) {
	sprintf(debug_str, "lrev %d", speed);
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(LPWM, constrain(speed, 0, 255 ));
      digitalWrite(LDir, HIGH);

}

void lcoast() {
	sprintf(debug_str, "lcoast");
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(LPWM, 0);
      digitalWrite(LDir, HIGH);
}

void lbrake() {
	sprintf(debug_str, "lbrake");
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(LPWM, 0);
      digitalWrite(LDir, LOW);
}



//////////////////////////////////////////////////////////////////////
//                               RIGHT
//////////////////////////////////////////////////////////////////////

void rfwd( int speed=255) {
	sprintf(debug_str, "rfwd %d", speed);
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(RPWM, constrain(speed, 0, 255 ));
      digitalWrite(RDir, LOW);

}

void rrev(int speed=255) {
	sprintf(debug_str, "rrev %d", speed);
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(RPWM, constrain(speed, 0, 255 ));
      digitalWrite(RDir, HIGH);

}


void rcoast() {
	sprintf(debug_str, "rcoast");
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(RPWM, 0);
      digitalWrite(RDir, HIGH);
}

void rbrake() {
	sprintf(debug_str, "rbrake");
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );


      analogWrite(RPWM, 0);
      digitalWrite(RDir, LOW);
}



//////////////////////////////////////////////////////////////////////
//                               CALLBACKS
//////////////////////////////////////////////////////////////////////


void RMotorCallBack( const std_msgs::Float32& motor_msg) {
	sprintf(debug_str, "LMotorCallback %0.3f", motor_msg.data);
	msg_debug.data = debug_str;
	debug_pub.publish( &msg_debug );

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

void setup(){  
  pinMode(10,  OUTPUT);                  //direction left
  pinMode(11,  OUTPUT);                  //PWM left
  pinMode(9,  OUTPUT);                  //PWM right
  pinMode(12,  OUTPUT);                  //direction right
  //circumference = pi*diameter;
 
  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.advertise(debug_pub);
  nh.advertise(lwheel_pub);
  nh.advertise(rwheel_pub);
  nh.subscribe(rmotor_sub);
  nh.subscribe(lmotor_sub);
}

void loop(){

  //msg_lwheel.data = lcoder;
  //msg_rwheel.data = rcoder;

  lwheel_pub.publish( &msg_lwheel );
  rwheel_pub.publish( &msg_rwheel );
  
  nh.spinOnce();
  delay(10);
}
