/*
 * base_teleop_joy.cpp
 *
 *  Created on: Dec 23, 2017
 *      Author: yusaku
 */

/*
 * base_teleop_joy.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: shun
 */


#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

static constexpr int LeftStickY = 1;
static constexpr int LeftStickX = 0;
static constexpr int RightStickX = 2;

class BaseTeleop
{
public:
    BaseTeleop();

private:
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

    ros::NodeHandle nh_;

    ros::Publisher vel_pub_;
    ros::Subscriber joy_sub_;

    double max_lin;
    double max_lin_turbo;
    double max_ang;  
    double max_ang_turbo;

        static int Button1;
	static int Button2;
	static int Button3;
	static int Button4;
	static int Button5;
	static int Button6;
	static int Button7;
	static int Button8;
	static int Button11;
	static int Button12;

        static int AxisLeftStickX;
	static int AxisLeftStickY;
	static int AxisRightStickX;
	static int AxisRightStickY;
	static int AxisJuziX;
	static int AxisJuziY;
};

int BaseTeleop::Button1 = 0;
int BaseTeleop::Button2 = 1;
int BaseTeleop::Button3 = 2;
int BaseTeleop::Button4 = 3;
int BaseTeleop::Button5 = 4;
int BaseTeleop::Button6 = 5;
int BaseTeleop::Button7 = 6;
int BaseTeleop::Button8 = 7;
int BaseTeleop::Button11 = 10;
int BaseTeleop::Button12 = 11;

int BaseTeleop::AxisLeftStickX = 0;
int BaseTeleop::AxisLeftStickY = 1;
int BaseTeleop::AxisRightStickX = 2;
int BaseTeleop::AxisRightStickY = 3;
int BaseTeleop::AxisJuziX = 4;
int BaseTeleop::AxisJuziY = 5;


BaseTeleop::BaseTeleop()
{

    vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);

    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &BaseTeleop::joyCallback, this);

    nh_.getParam("AxisLeftStickX", AxisLeftStickX);
    nh_.getParam("AxisLeftStickY", AxisLeftStickY);
    nh_.getParam("AxisRightStickX", AxisRightStickX);
    nh_.getParam("AxisRightStickY", AxisRightStickY);
    nh_.getParam("AxisJuziX", AxisJuziX);
    nh_.getParam("AxisJuziY", AxisJuziY);

    auto _nh = ros::NodeHandle("~");

    _nh.param("max_lin", this->max_lin, 1.0);
    _nh.param("max_lin_turbo", this->max_lin_turbo, this->max_lin);
    _nh.param("max_ang", this->max_ang, M_PI);
    _nh.param("max_ang_turbo", this->max_ang_turbo, this->max_ang);

    ROS_INFO("max_lin: %lf", this->max_lin);
    ROS_INFO("max_lin_turbo: %lf", this->max_lin_turbo);
    ROS_INFO("max_ang: %lf", this->max_ang);
    ROS_INFO("max_ang_turbo: %lf", this->max_ang_turbo);
}

void BaseTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    geometry_msgs::Twist twist;

    // double vel_x = joy->axes[AxisLeftThumbY];
    // double vel_y = joy->axes[AxisLeftThumbX];
    // double vel_z = joy->axes[AxisRightThumbX];

    double vel_x = joy->axes[AxisRightStickY];
    double vel_y = joy->axes[AxisLeftStickX];
    double vel_z = joy->axes[AxisJuziX];

    double vel_norm = hypot(vel_x, vel_y);
    if (vel_norm > 1.0)
    {
        vel_x /= vel_norm;
        vel_y /= vel_norm;
    }


        vel_x *= this->max_lin;
        vel_y *= this->max_lin;
        vel_z *= this->max_ang;


    twist.linear.x = vel_x;
    twist.linear.y = vel_y;
    twist.angular.z = vel_z;

    vel_pub_.publish(twist);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "base_teleop_joy");

    BaseTeleop baseTeleop;

    ros::spin();
}

