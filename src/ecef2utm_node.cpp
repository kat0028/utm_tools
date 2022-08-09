#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>

#include <string>
#include "utm_tools/utm_converter.hpp"

class ecef2utm
{
    private: 
    ros::NodeHandle n;

    ros::Subscriber odom_sub;
    ros::Publisher pose_pub;

    UTMConverter converter;
    UTMConverter::utm utm_coordinate;
    UTMConverter::ecef ecef_coordinate;
    
    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
    {
        ecef_coordinate.x = msg->pose.pose.position.x;
        ecef_coordinate.y = msg->pose.pose.position.y;
        ecef_coordinate.z = msg->pose.pose.position.z;
        converter.convert_ecef2utm(ecef_coordinate, utm_coordinate);      

        geometry_msgs::PoseStamped pose_msg;
        pose_msg.header.stamp = msg->header.stamp;
        pose_msg.pose.position.x = utm_coordinate.easting;
        pose_msg.pose.position.y = utm_coordinate.northing;
        pose_msg.pose.position.z = utm_coordinate.alt;

        pose_pub.publish(pose_msg);  
    }

    public:
    ecef2utm() : n("~")
    {
        std::string sub_topic;
        std::string pub_topic;
        n.param<std::string>("sub_topic", sub_topic, "/pose");
        n.param<std::string>("pub_topic", pub_topic, "/odom");

        n.subscribe(sub_topic, 1, &ecef2utm::odomCallback, this);
        n.advertise<geometry_msgs::PoseStamped>(pub_topic, 1);
    }

    ~ecef2utm()
    {
        // Do Nothing
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ecef2utm");
    ecef2utm converter_node;

    ros::spin();

    ros::shutdown();
    return 0;
}