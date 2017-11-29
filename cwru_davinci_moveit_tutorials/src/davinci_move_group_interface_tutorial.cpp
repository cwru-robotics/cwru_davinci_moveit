/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, SRI International
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of SRI International nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Su Lu */

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <moveit_visual_tools/moveit_visual_tools.h>


int main(int argc, char **argv) {
    ros::init(argc, argv, "move_group_interface_tutorial");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    //***********************************************************************************************************

    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr robot_model = robot_model_loader.getModel();
    ROS_INFO_NAMED("tutorial", "Frame in which the transforms for this model are computed: %s",
                   robot_model->getModelFrame().c_str());

    moveit::core::RobotState daVinciRobotState(robot_model);

    planning_scene::PlanningScenePtr planning_scene(new planning_scene::PlanningScene(robot_model));
    ROS_INFO_STREAM("\n" << planning_scene->getFrameTransform("/psm_one_tool_wrist_sca_shaft_link").matrix());
    ROS_INFO_STREAM("\n" << daVinciRobotState.getFrameTransform("/psm_one_tool_wrist_sca_shaft_link").matrix());

    //***********************************************************************************************************

    // BEGIN_TUTORIAL
    //
    // Setup
    // ^^^^^
    //
    // MoveIt! operates on sets of joints called "planning groups" and stores them in an object called
    // the `JointModelGroup`. Throughout MoveIt! the terms "planning group" and "joint model group"
    // are used interchangably.
    static const std::string PLANNING_GROUP = "psm_one";

    // The :move_group_interface:`MoveGroup` class can be easily
    // setup using just the name of the planning group you would like to control and plan for.
    moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);

    // We will use the :planning_scene_interface:`PlanningSceneInterface`
    // class to add and remove collision objects in our "virtual world" scene
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    // Raw pointers are frequently used to refer to the planning group for improved performance.
    const robot_state::JointModelGroup *joint_model_group =
            move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    // Visualization
    // ^^^^^^^^^^^^^
    //
    // The package MoveItVisualTools provides many capabilties for visualizing objects, robots,
    // and trajectories in Rviz as well as debugging tools such as step-by-step introspection of a script
    namespace rvt = rviz_visual_tools;
    moveit_visual_tools::MoveItVisualTools visual_tools("world");
    visual_tools.deleteAllMarkers();

    // Remote control is an introspection tool that allows users to step through a high level script
    // via buttons and keyboard shortcuts in Rviz
    visual_tools.loadRemoteControl();

    // Rviz provides many types of markers, in this demo we will use text, cylinders, and spheres
    Eigen::Affine3d text_pose = Eigen::Affine3d::Identity();

    text_pose.translation().z() = 1.75;  // a magic number need to be changed later

    visual_tools.publishText(text_pose, "daVinciMoveGroupInterface Demo", rvt::WHITE, rvt::XLARGE);

    // Batch publishing is used to reduce the number of messages being sent to Rviz for large visualizations
    visual_tools.trigger();

    // Getting Basic Information
    // ^^^^^^^^^^^^^^^^^^^^^^^^^
    //
    // We can print the name of the reference frame for this robot.
    ROS_INFO_NAMED("tutorial", "Reference frame: %s", move_group.getPlanningFrame().c_str());

    // We can also print the name of the end-effector link for this group.
    ROS_INFO_NAMED("tutorial", "End effector link: %s", move_group.getEndEffectorLink().c_str());


    // Planning to a Pose goal
    // ^^^^^^^^^^^^^^^^^^^^^^^
    // We can plan a motion for this group to a desired pose for the
    // end-effector.

    //**************************************************************************************************************

    Eigen::Affine3d sca_shaft_link_goal_pose;

    Eigen::Matrix3d orientaion;  // orientation part
    orientaion << 1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0;

    Eigen::Vector3d translation;  // translation part
    translation << 0.0, 0.05, 0.0;

    sca_shaft_link_goal_pose.linear() = orientaion;

    sca_shaft_link_goal_pose.translation() = translation;

    Eigen::Affine3d sca_shaft_link_wrt_world_frame = daVinciRobotState.getFrameTransform("/psm_one_tool_wrist_sca_shaft_link");

    Eigen::Affine3d sca_shaft_link_goal_pose_wrt_world = sca_shaft_link_wrt_world_frame * sca_shaft_link_goal_pose;

    ROS_INFO_STREAM("\n" << sca_shaft_link_goal_pose_wrt_world.matrix());

    move_group.setPoseTarget(sca_shaft_link_goal_pose_wrt_world);

    //**************************************************************************************************************
//    geometry_msgs::Pose target_pose_1;
//    target_pose_1.orientation.w = 1;
//    target_pose_1.orientation.x = -0.7071;
//    target_pose_1.orientation.y = 0;
//    target_pose_1.orientation.z = 0;
//
//
//    target_pose_1.position.x = 0.0;
//    target_pose_1.position.y = 0.05;
//    target_pose_1.position.z = 0.0;
//    move_group.setPoseTarget(target_pose_1);
//    move_group.setPoseReferenceFrame("psm_one_tool_wrist_sca_shaft_link");

    // Now, we call the planner to compute the plan and visualize it.
    // Note that we are just planning, not asking move_group
    // to actually move the robot.
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;

    bool success = move_group.plan(my_plan);

    ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "": "FAILED");

    // Visualizing plans
    // ^^^^^^^^^^^^^^^^^
    // We can also visualize the plan as a line with markers in Rviz.
    visual_tools.publishAxisLabeled(sca_shaft_link_goal_pose_wrt_world, "pose1");
    visual_tools.publishText(text_pose, "Pose Goal", rvt::WHITE, rvt::XLARGE);
    visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
    visual_tools.trigger();
    visual_tools.prompt("next step");

    // Moving to a pose goal
    // ^^^^^^^^^^^^^^^^^^^^^
    //
    // Moving to a pose goal is similar to the step above
    // except we now use the move() function. Note that
    // the pose goal we had set earlier is still active
    // and so the robot will try to move to that goal. We will
    // not use that function in this tutorial since it is
    // a blocking function and requires a controller to be active
    // and report success on execution of a trajectory.

    /* Uncomment below line when working with a real robot */
    /* move_group.move() */

    // Planning to a joint-space goal
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //
    // Let's set a joint space goal and move towards it.  This will replace the
    // pose target we set above.
    //
    // To start, we'll create an pointer that references the current robot's state.
    // RobotState is the object that contains all the current position/velocity/acceleration data.
    moveit::core::RobotStatePtr current_state = move_group.getCurrentState();
    //
    // Next get the current set of joint values for the group.
    std::vector<double> joint_group_positions;
    current_state -> copyJointGroupPositions( joint_model_group, joint_group_positions);

    for ( int i = 0; i < joint_group_positions.size(); i++ ){
        ROS_INFO("%dth joint value: %f \n", i, joint_group_positions[i]);
    }

    // Now, let's modify one of the joints, plan to the new joint space goal and visualize the lan.
    joint_group_positions[1] = -0.5;  // radians
    move_group.setJointValueTarget(joint_group_positions);
    success = move_group.plan(my_plan);

    ROS_INFO_NAMED("tutorial", "Visualizing plan 2 (joint space goal) %s", success ? "" : "FAILED");

    // Visualize the plan in Rviz
    visual_tools.deleteAllMarkers();
    visual_tools.publishText(text_pose, "Joint Space Goal", rvt::WHITE, rvt::XLARGE);
    visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
    visual_tools.trigger();
    visual_tools.prompt("next step");

    // Adding/Removing Objects and Attaching/Detaching Objects
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //
    // Define a collision object ROS message.
    moveit_msgs::CollisionObject collision_object;
    collision_object.header.frame_id = move_group.getPlanningFrame();

    // The id of the object is used to identify it.
    collision_object.id = "box1";

    // Define a box to add to the world.
    shape_msgs::SolidPrimitive primitive;
    primitive.type = primitive.BOX;
    primitive.dimensions.resize(3);
    primitive.dimensions[0] = 0.005;
    primitive.dimensions[1] = 0.005;
    primitive.dimensions[2] = 0.005;

    // Define a pose for the box (specified relative to frame_id)
//    Eigen::Affine3d box_pose;
//
//    Eigen::Matrix3d box_orientaion;  // orientation part
//    box_orientaion << 1.0, 0.0, 0.0,
//            0.0, 1.0, 0.0,
//            0.0, 0.0, 1.0;
//
//    Eigen::Vector3d box_translation;  // translation part
//    translation << 0.0, 0.5, 0.0;
//
//    box_pose.linear() = box_orientaion;
//
//    box_pose.translation() = box_translation;
//
////    Eigen::Affine3d sca_shaft_link_wrt_world_frame = daVinciRobotState.getFrameTransform("/psm_one_tool_wrist_sca_shaft_link");
//
//    Eigen::Affine3d box_pose_wrt_world_eigen = sca_shaft_link_wrt_world_frame * box_pose;
//
//    geometry_msgs::Pose box_pose_wrt_world;
//    Eigen::Quaternion<double> pose_q(box_pose_wrt_world_eigen.rotation());
//
//    box_pose_wrt_world.orientation.w = pose_q.w();
//    box_pose_wrt_world.orientation.x = pose_q.x();
//    box_pose_wrt_world.orientation.y = pose_q.y();
//    box_pose_wrt_world.orientation.z = pose_q.z();
//
//    box_pose_wrt_world.position.x = box_pose_wrt_world_eigen.translation().x();
//    box_pose_wrt_world.position.y = box_pose_wrt_world_eigen.translation().y();
//    box_pose_wrt_world.position.z = box_pose_wrt_world_eigen.translation().z();

    //Define a pose for the box (specified relative to frame_id)
    geometry_msgs::Pose box_pose_wrt_world;
    box_pose_wrt_world.orientation.w = 1.0;
    box_pose_wrt_world.position.x = -0.25;
    box_pose_wrt_world.position.y = 0.0;
    box_pose_wrt_world.position.z = 0.48;

    collision_object.primitives.push_back(primitive);
    collision_object.primitive_poses.push_back(box_pose_wrt_world);
    collision_object.operation = collision_object.ADD;

    std::vector<moveit_msgs::CollisionObject> collision_objects;
    collision_objects.push_back(collision_object);

    // Now, let's add the collision object into the world
    ROS_INFO_NAMED("tutorial", "Add an object into the world");
    planning_scene_interface.addCollisionObjects(collision_objects);

    // Show text in Rviz of status
    visual_tools.publishText(text_pose, "Add object", rvt::WHITE, rvt::XLARGE);
    visual_tools.trigger();

    // Sleep to allow MoveGroup to recieve and process the collision object message
    ros::Duration(1.0).sleep();
    // Now when we plan a trajectory it will avoid the obstacle
    move_group.setStartState(*move_group.getCurrentState());


    orientaion << 1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0;

    translation << 0.0, 0.05, -0.02;

    sca_shaft_link_goal_pose.linear() = orientaion;

    sca_shaft_link_goal_pose.translation() = translation;

//    Eigen::Affine3d sca_shaft_link_wrt_world_frame = daVinciRobotState.getFrameTransform("/psm_one_tool_wrist_sca_shaft_link");

    sca_shaft_link_goal_pose_wrt_world = sca_shaft_link_wrt_world_frame * sca_shaft_link_goal_pose;

    ROS_INFO_STREAM("\n" << sca_shaft_link_goal_pose_wrt_world.matrix());


    move_group.setPoseTarget(sca_shaft_link_goal_pose_wrt_world);

    success = move_group.plan(my_plan);
    ROS_INFO_NAMED("tutorial", "Visualizing plan 5 (pose goal move around cuboid) %s", success ? "" : "FAILED");

    // Visualize the plan in Rviz
    visual_tools.deleteAllMarkers();
    visual_tools.publishText(text_pose, "Obstacle Goal", rvt::WHITE, rvt::XLARGE);
    visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
    visual_tools.trigger();
    visual_tools.prompt("next step");
    // END_TUTORIAL

    ros::shutdown();
    return 0;
}