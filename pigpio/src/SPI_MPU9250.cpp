#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "SPI_MPU9250");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  int i = 0;
  unsigned int spi_channel = 1;
  unsigned int baud = 1000000;
  unsigned int spi_flags = 0;
  
  //spi_flags setting
  spi_flags = spi_flags | 0b00;			//select mm
  spi_flags = spi_flags | (0b000<<2);	//select p0-p3 
  spi_flags = spi_flags | (0b000<<5);	//select u0-u3
  spi_flags = spi_flags | (0b0<<8);		//select A
  spi_flags = spi_flags | (0b0<<9);		//select W
  spi_flags = spi_flags | (0b0001<<10);	//select n
  spi_flags = spi_flags | (0b0<<14);	//select T
  spi_flags = spi_flags | (0b0<<15);	//select R
  spi_flags = spi_flags | (0b1000<<16);	//select b
  ROS_INFO("%d",spi_flags);
  ROS_INFO("%d",pi);
  i = spi_open(pi,spi_channel,baud,spi_flags);

  ROS_INFO("%d",i);
/*
  ros::Rate rate(10);
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
