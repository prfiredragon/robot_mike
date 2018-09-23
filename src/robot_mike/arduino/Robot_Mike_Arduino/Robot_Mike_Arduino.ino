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
#include <Time.h>

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
                               LEFT
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
                               RIGHT
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
                               CALLBACKS
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void RMotorCallBack( const std_msgs::Float32& motor_msg) {
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
void LMotorCallBack( const std_msgs::Float32& motor_msg) {
//////////////////////////////////////////////////////////////////////

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


void runCommand(float linear, float angular){
  //const float FLRev=1,RbWidth=0.44;           // Experimentally find the FULL LOAD Revolutions for 1 second     // 1 means 0.5 m/s, because wheel length is 50cm
  //int LinPWMval=2*linX*255/FLRev;             // Calculate the PWM value for received linear velocity command
  //int AngPWMval=2*angZ*RbWidth*255/FLRev;
  int Rwheel=0;
  int Lwheel=0;


  if(linX>0){
      Rwheel=55;
      Lwheel=55;
  }
  
  if(linX<0){
      Rwheel=0;
      Lwheel=0;
  }
  if(angZ>0){
      Rwheel=55;
      Lwheel=-55;
  }
  
  if(angZ<0){
      Rwheel=-55;
      Lwheel=55;
  }
/*  const float FLRev=1;           // Experimentally find the FULL LOAD Revolutions for 1 second     // 1 means 0.5 m/s, because wheel length is 50cm
  int LinPWMval=2*linX*circumference*255/FLRev;             // Calculate the PWM value for received linear velocity command
  int AngPWMval=angZ*circumference*255/FLRev;
  int Rwheel=LinPWMval+AngPWMval;
  int Lwheel=LinPWMval-AngPWMval;
*/
  if(Rwheel>0){
      analogWrite(RPWM, Rwheel);
      digitalWrite(RDir, LOW);
  }
  else{
      analogWrite(RPWM, -Rwheel);
      digitalWrite(RDir, HIGH);
  }

  if(Lwheel>0){
      analogWrite(LPWM, Lwheel);
      digitalWrite(LDir, LOW);
  }
  else{
      analogWrite(LPWM, -Lwheel);
      digitalWrite(LDir, HIGH);
  }

}


























































/*  This is line number 200, Special thanks for python & shell scripting to 

    SPNP Subasinghe, 
    Department of Computer Science & Technology, 
    Faculty of Science & Technology, 
    Uva Wellassa University of Sri Lanka. 
    
    By -Author-    10/01/2014                   			   */
