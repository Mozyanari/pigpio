#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "GPIO_uart_test");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  //int serial_flag = serial_open(pi,"/dev/ttyUSB0",115200,0);
  int serial_flag = serial_open(pi,"/dev/ttyAMA0",115200,0);
  if(serial_flag < 0){
		ROS_INFO("Fail %d",serial_flag);
  }else{
		ROS_INFO("Success %d",serial_flag);
  }
  return 0;
}
