//=================================================================================================
// Copyright (c) 2012, Mark Sollweck, Stefan Kohlbrecher, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Simulation, Systems Optimization and Robotics
//       group, TU Darmstadt nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#ifndef HECTOR_EXPLORATION_PLANNER_H___
#define HECTOR_EXPLORATION_PLANNER_H___

#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point.h>
#include <vector>
#include <costmap_2d/costmap_2d.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <string>
#include <ros/exceptions.h>
#include <math.h>
#include <tf/tf.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <boost/lexical_cast.hpp>
#include <hector_nav_msgs/GetRobotTrajectory.h>
#include <stdlib.h>

#define STRAIGHT_COST 3
#define DIAGONAL_COST 4
#define _USE_MATH_DEFINES

namespace hector_exploration_planner{

class HectorExplorationPlanner {
public:
  HectorExplorationPlanner();
  ~HectorExplorationPlanner();
  HectorExplorationPlanner(std::string name,costmap_2d::Costmap2DROS *costmap_ros);
  void initialize(std::string name,costmap_2d::Costmap2DROS *costmap_ros);
  bool makePlan(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal,std::vector<geometry_msgs::PoseStamped> &plan);
  bool doExploration(const geometry_msgs::PoseStamped &start,std::vector<geometry_msgs::PoseStamped> &plan);
  bool doInnerExploration(const geometry_msgs::PoseStamped &start,std::vector<geometry_msgs::PoseStamped> &plan);
  bool doAlternativeExploration(const geometry_msgs::PoseStamped &start,std::vector<geometry_msgs::PoseStamped> &plan, std::vector<geometry_msgs::PoseStamped> &oldplan);
  bool findFrontiers(std::vector<geometry_msgs::PoseStamped> &frontiers, std::vector<geometry_msgs::PoseStamped> &noFrontiers);
  bool findFrontiers(std::vector<geometry_msgs::PoseStamped> &frontiers);
  bool findInnerFrontier(std::vector<geometry_msgs::PoseStamped> &innerFrontier);

private:
  bool buildObstacleTrans();
  bool buildExplorationTrans(const geometry_msgs::PoseStamped &start, std::vector<geometry_msgs::PoseStamped> goals,bool useAnglePenalty);
  bool getTrajectory(const geometry_msgs::PoseStamped &start, std::vector<geometry_msgs::PoseStamped> goals, std::vector<geometry_msgs::PoseStamped> &plan);
  bool recoveryMakePlan(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal,std::vector<geometry_msgs::PoseStamped> &plan);
  unsigned int cellDanger(int point);
  unsigned int angleDanger(float angle);

  void saveMaps(std::string path);
  void resetMaps();
  void clearFrontiers();
  bool isValid(int point);
  bool isFree(int point);
  bool isFrontier(int point);
  float angleDifference(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal);
  float getDistanceWeight(const geometry_msgs::PoseStamped &point1, const geometry_msgs::PoseStamped &point2);
  double getYawToUnknown(int point);
  bool isFrontierReached(int point);
  bool isSameFrontier(int frontier_point1,int frontier_point2);

  void getStraightPoints(int point, int points[]);
  void getDiagonalPoints(int point, int points[]);
  void getAdjacentPoints(int point, int points[]);
  int left(int point);
  int upleft(int point);
  int up(int point);
  int upright(int point);
  int right(int point);
  int downright(int point);
  int down(int point);
  int downleft(int point);



  ros::Publisher visualization_pub_;
  ros::ServiceClient path_service_client_;
  costmap_2d::Costmap2DROS* costmap_ros;
  costmap_2d::Costmap2D costmap;
  const unsigned char* occupancyGrid;
  unsigned int* explorationTrans;
  unsigned int* obstacleTrans;
  int* frontierMap;
  bool* isGoal;

  bool initialized;
  bool planInUnknown;
  bool useInflatedObs;
  int goal_angle_penalty;
  std::string name;
  unsigned int mapWidth;
  unsigned int mapHeight;
  unsigned int mapPoints;
  int minObstacleDist;
  int minFrontierSize;
  double alpha;
  int previous_goal;
  double dist_for_goal_reached;
  double same_frontier_dist;
};
}

#endif

