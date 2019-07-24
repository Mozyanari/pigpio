#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>
//#include <iostream>
//#include <chrono>
//#include <thread>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "uart");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  int i = 0;

  int flag = serial_open(pi,"/dev/serial0",115200,0);
  
  ROS_INFO("%d",flag);
/*
  ros::Rate rate(1);
  while(ros::ok()){
	if(i == 0){
      gpio_write(pi,26,1);
	  i = 1;
	}else{
	  gpio_write(pi,26,0);
	  i = 0;
	}
	ROS_INFO("%d",i);
	ros::spinOnce();
	rate.sleep();
  }
*/
  return 0;
}
