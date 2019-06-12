#include "ros/ros.h"
#include "marvelmind_nav/hedge_pos_ang.h"
#include "tf/transform_broadcaster.h"
#include <nav_msgs/Odometry.h>

class marvelmind_tf  //marvelmind_tf
{
public:
  marvelmind_tf() // marvelmind_tf
  {
    ros::NodeHandle nh_; 
    
    // Init subscribers
    pos_ang_sub_ = nh_.subscribe("hedge_pos_ang", 10, &marvelmind_tf::pos_ang_callback, this);

    // Init publishers
    hedge_pose_pub_ = nh_.advertise<nav_msgs::Odometry>("gps", 50);   
 
    pose_out_.header.frame_id = "map";
    pose_out_.child_frame_id = "base_footprint";
  }

  void pos_ang_callback(const marvelmind_nav::hedge_pos_ang::ConstPtr& pos_ang_msg)
  {

    pose_out_.header.stamp = ros::Time::now();

    pose_out_.pose.pose.position.x = pos_ang_msg->x_m;
    pose_out_.pose.pose.position.y = pos_ang_msg->y_m;
    pose_out_.pose.pose.position.z = pos_ang_msg->z_m;

    pose_out_.pose.covariance[0] = 1;
    pose_out_.pose.covariance[7] = 1;
    pose_out_.pose.covariance[14] = 1;
    pose_out_.pose.covariance[21] = 1;
    pose_out_.pose.covariance[28] = 1;
    pose_out_.pose.covariance[35] = 1;

    pose_out_.pose.pose.orientation = tf::createQuaternionMsgFromYaw(pos_ang_msg->angle);
	
    ROS_INFO("X=%.3f  Y= %.3f  Z=%.3f  ", 	
				//(int) hedge_pos_msg.address, 
				//(int) hedge_pos_msg.timestamp_ms, 
				(float) pose_out_.pose.pose.position.x, (float) pose_out_.pose.pose.position.y, (float) pose_out_.pose.pose.position.z);

    hedge_pose_pub_.publish(pose_out_);
  }


protected:

  ros::Subscriber pos_ang_sub_;
  ros::Publisher hedge_pose_pub_;

  // Message objects
  nav_msgs::Odometry pose_out_;
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "marvelmind_tf");

  marvelmind_tf adapter;

  ros::spin();
}

