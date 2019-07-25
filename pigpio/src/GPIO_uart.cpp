#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "GPIO_uart");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  char i = 0;

  //int serial_flag = serial_open(pi,"/dev/ttyUSB0",115200,0);
  int serial_flag = serial_open(pi,"/dev/ttyAMA0",115200,0);
  if(serial_flag < 0){
	return 0;
  }else{
	ros::Rate rate(100);
 	while(ros::ok()){
	  serial_write(pi,serial_flag,&i,sizeof(i));
  	  ROS_INFO("%d",i);
	  i++;
	  if(i>200){
		i=0;
	  }
	  ros::spinOnce();
	  rate.sleep();
	}
  }
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
