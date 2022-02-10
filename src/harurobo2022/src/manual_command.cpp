/*
// quitaに載ってるros講座07のコードを参考にした
*/


#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float32.h>
#include "harurobo2022/config.hpp"

class ManualCommand{
public:
  ManualCommand(){
    cmd_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    joy_sub_ = nh_.subscribe("joy", 10, &ManualCommand::joyCallback, this);
    timer_ = nh_.createTimer(ros::Duration(0.1), &ManualCommand::timerCallback, this);
  }

  void joyCallback(const sensor_msgs::Joy& joy_msg) {
    last_joy_ = joy_msg;
  }

  void timerCallback(const ros::TimerEvent& e) {
    
        geometry_msgs::Twist cmd_vel;
    if(0 < last_joy_.axes.size()){
      cmd_vel.linear.x = Config::max_body_vell_x * last_joy_.axes[0];   //[0]はコントローラーの左スティック左右の割り当て
    }
    if(1 < last_joy_.axes.size()){
      cmd_vel.linear.y = Config::max_body_vell_y * last_joy_.axes[1];   //[1]はコントローラーの左スティック上下の割り当て
    }
    if(2 < last_joy_.axes.size()){
      cmd_vel.angular.z =Config::max_body_vela_z * last_joy_.axes[2];   //[2]はコントローラーの右スティック左右の割り当て
    }
    cmd_pub_.publish(cmd_vel);
  }

  ros::NodeHandle nh_;
  ros::Publisher cmd_pub_;
  ros::Subscriber joy_sub_;
  ros::Timer timer_;
  sensor_msgs::Joy last_joy_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "manual_command");

  ManualCommand manual_command;
  
  ROS_INFO("manual_command node has started.");
  
  ros::spin();
  
  ROS_INFO("manual_command node has terminated.");
  
  return 0;
}
