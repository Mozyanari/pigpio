#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>

#include <std_msgs/Bool.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "SPI_test");
  
  ros::NodeHandle n;

  int pi;
  pi = pigpio_start(NULL,NULL);

  int spi_handle = 0;
  unsigned int spi_channel = 0;			//0ch
  unsigned int baud = 1000000;			//1MHz
  unsigned int spi_flags = 0;

  char data = 0;
  
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
  spi_handle = spi_open(pi,spi_channel,baud,spi_flags);

  ROS_INFO("%d",spi_handle);

  ros::Rate rate(10);
  while(ros::ok()){
	int result = spi_write(pi,spi_handle,&data,1);
	ROS_INFO("%d",result);
	data++;
	if(data>200){
		data = 0;
	}
	
	ros::spinOnce();
	rate.sleep();
  }
  return 0;
}
