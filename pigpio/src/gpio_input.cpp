#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>
//#include <iostream>
//#include <chrono>
//#include <thread>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "gpio_input");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  int i = 0;

  ros::Rate rate(10);
  while(ros::ok()){
	int input = gpio_read(pi,19);
	if(input == 1){
      gpio_write(pi,26,1);
	}else{
	  gpio_write(pi,26,0);
	}
	ROS_INFO("%d",input);
	ros::spinOnce();
	rate.sleep();
  }
  return 0;
}
