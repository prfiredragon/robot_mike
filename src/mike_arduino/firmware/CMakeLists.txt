cmake_minimum_required(VERSION 2.8.3)

include_directories(${ROS_LIB_DIR})

# Remove this if using an Arduino without native USB (eg, other than Leonardo)
add_definitions(-DUSB_CON)

generate_arduino_firmware(mike
  SRCS mike.cpp ${ROS_LIB_DIR}/time.cpp
  BOARD uno
  PORT /dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_6493330373235170D010-if00
)
