#include <ros/ros.h>
#include <pigpiod_if2.h>
#include <std_msgs/Bool.h>

class gpio_topic{
  //construct
  public:
  gpio_topic();
  
  private:
  int pi;
  
  void cb_LED(const std_msgs::Bool::ConstPtr &data);
  ros::NodeHandle nh;
  ros::Subscriber sub_led;
};

gpio_topic::gpio_topic(){
  pi = pigpio_start(NULL,NULL);

  sub_led = nh.subscribe("/led", 5, &gpio_topic::cb_LED, this);
}

void gpio_topic::cb_LED(const std_msgs::Bool::ConstPtr &data){
  if(data->data == 1){
    gpio_write(pi,26,1);
  }else{
    gpio_write(pi,26,0);
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "gpio");
  gpio_topic gpio_topic;
  ros::spin();
  return 0;
}