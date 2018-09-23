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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                                              ////
////                    Author    :     RATHNAYAKE HGHB                                                           ////
////                                                                                                              ////
////                    Department:     SCIENCE & TECHNOLOGY                                                      ////
////                                                                                                              ////
////                    Faculty   :     SCIENCE & TECHNOLOGY                                                      ////
////                                                                                                              ////
////                    Institute :     UVA WELLASSA UNIVERSITY OF SRI LANKA                                      ////
////                                                                                                              ////               
////                    File Name :     UWU_BOT.INO, SUPPORT FOR ROSSERIAL                                        ////
////                                                                                                              ////
////                    Project   :     MODELING 3D ENVIRONMENT USING ROS, KINECT & ARDUINO                       ////
////                                                                                                              ////
////                    Credits   :     PIYATHILAKA JMLC, EKANAYAKE RMTCB, ROSSERIAL TEAM                         ////
////                                                                                                              ////
////                    E-mail    :     HASITHARATHNAYAKE@LIVE.COM                                                ////
////                                                                                                              ////
////                    Phone     :     +94711222022                                                              ////
////                                                                                                              ////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                        //
//     Copyright (c) 2014, HasithaBR, Uva Wellassa University                                                             //  
//     All rights reserved.                                                                                               //
//                                                                                                                        //
//     Redistribution and use in source and binary forms, with or without modification, are permitted provided that the   //
//     following conditions are met:                                                                                      //
//                                                                                                                        //
//      * Redistributions of source code must retain the above copyright notice, this list of conditions and the          //
//        following disclaimer.                                                                                           //
//      * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       //
//        following disclaimer in the documentation and/or other materials provided with the distribution.                //
//      * Neither the name of Uva Wellassa University nor the names of its contributors may be used to endorse or promote //
//        products derived from this software without specific prior written permission.                                  //
//                                                                                                                        //
//     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, //
//     INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  //
//     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  //
//     SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR    //
//     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,  //
//     WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE   //
//     USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                           //
//                                                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <Time.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

#define LDir 10
#define LPWM 11
#define RDir 12
#define RPWM 9

ros::NodeHandle nh;

float linX=0,angZ=0,Dis=0;              // Initiallize global variables that handles linear and angular velocities here
long time=0;
//int pi=3.141592654;
//int diameter=0.1524;
//int circumference=0;

void messageVel(const geometry_msgs::Twist& msg){
  linX = msg.linear.x;                  // Linear velocity component of received twist message
  angZ = msg.angular.z;                 // Angular velocity component of received twist message
}

ros::Subscriber<geometry_msgs::Twist> Robot_Mike("cmd_vel", &messageVel);  // Keyboard teleoperation velocity commander

void setup(){  
  pinMode(10,  OUTPUT);                  //direction left
  pinMode(11,  OUTPUT);                  //PWM left
  pinMode(9,  OUTPUT);                  //PWM right
  pinMode(12,  OUTPUT);                  //direction right
  //circumference = pi*diameter;
 
  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.subscribe(Robot_Mike);
}

void loop(){
  runCommand(linX,angZ);
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
