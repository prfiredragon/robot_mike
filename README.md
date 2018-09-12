# **Robot Mike**
My own robot experiment with ROS

## **Status:** Currently working on the wheels.

## **Hardware:**

* Arduino uno
* Odroid-c2 with 2GB RAM HDMI 2.0 IR Gigabit
* Accessory USA 5v USB Pc Charging Cable Lead Power Supply Cord for Android Tablet Pc & More Od: 2.5mm Id: 0.8mm (2.5mm X 0.8mm) 2.5x0.8 Dc Barrel Plug Seires
* 16GB microSD
* kinect (xbox 360)
* Agile-Shop DC Converter Buck Module 12V convert to 5V USB Output Power Adapter
* 2x 6" HiGrip Rubber Wheel (Tetrix) (RB-And-111)
* 2x 12V DC Motor 146RPM w/Encoder (RB-Dfr-439)
* Cytron 10A 5-30V Dual Channel DC Motor Driver (RB-Cyt-153)
* Stepper Motor Mounting Bracket (NEMA 17) (RB-Phi-273)
* 2x 6mm Set Screw Hub (RB-Nex-17)

## **Software:**

### **Not Included:**

* ROS
* image_geometry

### **Included:**

* depthimage_to_laserscan
* rosserial

## **Software Instructions**
1. install ROS
2. run `git clone https://github.com/prfiredragon/robot_mike`
3. run `cd robot_mike`
4. run `sudo apt-get install ros-melodic-image-geometry`
5. run `catkin_make`
6. run `echo "source /path to robot_mike/setup.bash" >> ~/.bashrc`
7. run `source ~/.bashrc`