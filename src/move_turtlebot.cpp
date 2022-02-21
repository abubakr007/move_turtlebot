#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
	// Initialize the simple navigation goals node
	ros::init(argc, argv, "move_turtlebot");
	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);
	// Wait 5 sec for move base action server to come up
	while(!ac.waitForServer (ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}
	move_base_msgs::MoveBaseGoal goal;
	// set up the frame parameters
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	float goals[2][3] =  {{-2.0, 0.0, 1.57}, {0.0, 0.0 , 1.57}};
	for (int i =0; i< 2; i++){
		// Define a position and orientation for the robot to reach
		goal.target_pose.pose.position.x = goals[i][0];
		goal.target_pose.pose.position.y = goals[i][1];
		goal.target_pose.pose.orientation.w = goals[i][2];
		// Send the goal position and orientation for the robot to reach
		ROS_INFO ("Sending goal");
		ac.sendGoal (goal);
		// Wait an infinite time for the results
		ac.waitForResult();
		ros::Duration(5.0).sleep();
	}
	// Check if the robot reached ts goal
	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO( "Hooray, reached drop off zone");
	else
		ROS_INFO("The base failed to move forward");
	return 0;
}