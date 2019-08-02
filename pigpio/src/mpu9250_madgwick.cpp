#include <ros/ros.h>
//#include <pigpio.h> 
#include <pigpiod_if2.h>

#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <sensor_msgs/Imu.h>

//SPI Pin setting
#define CS_pin 8

//MPU9250 address
#define set_bit_1 0x80
#define who_am_i 0x75
#define wake_up_1 0x6B
#define wake_up_ward_1 0x00
#define wake_up_2 0x37
#define wake_up_ward_2 0x02

//x方向の加速度
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
//y方向の加速度
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
//z方向の加速度
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
//raw軸方向の角加速度
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
//pitch軸方向の角加速度
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
//yaw軸方向の角加速度
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

class SPI_MPU9250
{
  public:
  SPI_MPU9250();
  int pigpio_initialize();
  int spi_initialize();
  void who_am_i_check();
  void mpu9250_power_on();
  int16_t get_acc_x();
  int16_t get_acc_y();
  int16_t get_acc_z();
  int16_t get_gyr_x();
  int16_t get_gyr_y();
  int16_t get_gyr_z();
  

  private:
  ros::NodeHandle n;
  char pigpio_spi_read(char address);
  char pigpio_spi_write(char address, char write_data);
  int pigpio_check;
  int spi_handle;
};

SPI_MPU9250::SPI_MPU9250(){
  
}

int SPI_MPU9250::pigpio_initialize(){
  pigpio_check = pigpio_start(NULL,NULL);
  if(pigpio_check < 0){
	  ROS_ERROR("pigpio Initialize Error");
	  throw;
  }else{
	  ROS_INFO("pigpio initialize OK");
	  return pigpio_check;
  }
}

int SPI_MPU9250::spi_initialize(){
  //spi config
  unsigned int spi_channel = 0;			//0ch
  unsigned int baud = 1000000;			//1MHz
  
  //spi_flags setting
  unsigned int spi_flags = 0;
  spi_flags = spi_flags | 0b00;			//select mm
  spi_flags = spi_flags | (0b000<<2);	//select p0-p3 
  spi_flags = spi_flags | (0b000<<5);	//select u0-u3
  spi_flags = spi_flags | (0b0<<8);		//select A
  spi_flags = spi_flags | (0b0<<9);		//select W
  spi_flags = spi_flags | (0b0001<<10);	//select n
  spi_flags = spi_flags | (0b0<<14);	//select T
  spi_flags = spi_flags | (0b0<<15);	//select R
  spi_flags = spi_flags | (0b1000<<16);	//select b
  //ROS_INFO("%d",spi_flags);

  //spi open
  spi_handle = spi_open(pigpio_check,spi_channel,baud,spi_flags);
  if(spi_handle < 0){
	  ROS_ERROR("SPI Initialize Error");
	  throw;
  }else{
    ROS_INFO("SPI initialize OK");
	  return spi_handle;
  } 
}

char SPI_MPU9250::pigpio_spi_read(char address){
  char read_data;
  char send_address = address | set_bit_1; 
  gpio_write(pigpio_check,CS_pin,0);
  spi_write(pigpio_check,spi_handle,&send_address,1);
  spi_read(pigpio_check,spi_handle,&read_data,1);
  gpio_write(pigpio_check,CS_pin,1);
  return read_data;
}

char SPI_MPU9250::pigpio_spi_write(char address, char write_data){
  char send_address = address;
  gpio_write(pigpio_check,CS_pin,0);

  char result1 = spi_write(pigpio_check,spi_handle,&address,1);
  char result2 = spi_write(pigpio_check,spi_handle,&write_data,1);
  gpio_write(pigpio_check,CS_pin,1);
  if(result1 < 1 | result2 < 1){
	  ROS_ERROR("write error result = %d %d",(int)result1,(int)result2);
	  throw;
  }else{
    ROS_INFO("MPU9250 OK");
	return 1;
  }
}

void SPI_MPU9250::who_am_i_check(){
  char read_data = pigpio_spi_read(who_am_i);
  if(read_data == 0x71){
	  ROS_INFO("who am i OK");
  }else{
	  ROS_ERROR("who am i Error read data = %d",(int)read_data);
	throw;
  }
}

void SPI_MPU9250::mpu9250_power_on(){
  pigpio_spi_write(wake_up_1,wake_up_ward_1);
  pigpio_spi_write(wake_up_2,wake_up_ward_2);
}

int16_t SPI_MPU9250::get_acc_x(){
  int acc_x_h = pigpio_spi_read(ACCEL_XOUT_H);
  int acc_x_l = pigpio_spi_read(ACCEL_XOUT_L);
  int16_t acc_x = ((acc_x_h << 8) | acc_x_l);
  return acc_x;
}

int16_t SPI_MPU9250::get_acc_y(){
  int acc_y_h = pigpio_spi_read(ACCEL_YOUT_H);
  int acc_y_l = pigpio_spi_read(ACCEL_YOUT_L);
  int16_t acc_y = ((acc_y_h << 8) | acc_y_l);
  return acc_y;
}

int16_t SPI_MPU9250::get_acc_z(){
  int acc_z_h = pigpio_spi_read(ACCEL_ZOUT_H);
  int acc_z_l = pigpio_spi_read(ACCEL_ZOUT_L);
  int16_t acc_z = ((acc_z_h << 8) | acc_z_l);
  return acc_z;
}

int16_t SPI_MPU9250::get_gyr_x(){
  int gyr_x_h = pigpio_spi_read(GYRO_XOUT_H);
  int gyr_x_l = pigpio_spi_read(GYRO_XOUT_L);
  int16_t gyr_x = ((gyr_x_h << 8) | gyr_x_l);
  return gyr_x;
}

int16_t SPI_MPU9250::get_gyr_y(){
  int gyr_y_h = pigpio_spi_read(GYRO_YOUT_H);
  int gyr_y_l = pigpio_spi_read(GYRO_YOUT_L);
  int16_t gyr_y = ((gyr_y_h << 8) | gyr_y_l);
  return gyr_y;
}

int16_t SPI_MPU9250::get_gyr_z(){
  int gyr_z_h = pigpio_spi_read(GYRO_ZOUT_H);
  int gyr_z_l = pigpio_spi_read(GYRO_ZOUT_L);
  int16_t gyr_z = ((gyr_z_h << 8) | gyr_z_l);
  return gyr_z;
}

int main(int argc, char** argv){
  ros::init(argc, argv, "SPI_MPU9250");
  
  SPI_MPU9250 mpu9250;
  //pigpio initialize
  mpu9250.pigpio_initialize();

  //spi initialze
  mpu9250.spi_initialize();
  //ROS_INFO("%d",spi_handle);

  //who_am_i check
  mpu9250.who_am_i_check();

  //MPU9250 power on
  mpu9250.mpu9250_power_on();
  
  ros::Rate rate(1000);
  while(ros::ok()){
    double acc_x = mpu9250.get_acc_x()/16384.0*9.81;
    double acc_y = mpu9250.get_acc_y()/16384.0*9.81;
	double acc_z = mpu9250.get_acc_z()/16384.0*9.81;
	double gyr_x = mpu9250.get_gyr_x()/130.072;
	double gyr_y = mpu9250.get_gyr_y()/130.072;
	double gyr_z = mpu9250.get_gyr_z()/130.072;
	ROS_INFO("acc_x = %f",acc_x);
	ROS_INFO("acc_y = %f",acc_y);
	ROS_INFO("acc_z = %f",acc_z);
	ROS_INFO("gyr_x = %f",gyr_x);
	ROS_INFO("gyr_y = %f",gyr_y);
	ROS_INFO("gyr_z = %f",gyr_z);

	sensor_msgs::Imu Imu;
	Imu.linear_acceleration.x = acc_x;
	Imu.linear_acceleration.y = acc_y;
	Imu.linear_acceleration.z = acc_z;

	Imu.

    /* 
	  char acc_z_h = pigpio_spi_read(ACCEL_ZOUT_H);
	  char acc_z_l = pigpio_spi_read(ACCEL_ZOUT_L);
	  double acc_z = ((z_up << 8) | z_down)/32768.0*2.0;
    */
	  //ROS_INFO("%d",z);
/*
	char who_am_i = 0x75 | 0x80;
	char receive_data;
	gpio_write(pi,8,0);
	spi_write(pi,spi_handle,&who_am_i,1);
	spi_read(pi,spi_handle,&receive_data,1);
	//int result = spi_xfer(pi,spi_handle,&who_am_i,&receive_data,1);
	gpio_write(pi,8,1);
	ROS_INFO("%d",receive_data);
	receive_data = 0;
*/
	/*
	int result = spi_write(pi,spi_handle,&data,1);
	ROS_INFO("%d",result);
	data++;
	if(data>200){
		data = 0;
	}
	*/
	  ros::spinOnce();
	  rate.sleep();
  }
  return 0;
}
