/*
 * ALLSPACE_VARIABLES.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */



#include "Node.h"
#include "Ant.h"
#include "Edge.h"
#include "JointPoint.h"
#include "Solution.h"
#include "ALLSPACE_VARIABLES.h"
#include <map>

 int bfsNum;
 int ant_id;      //问题  //ant_id没有赋初值，所以其初值为0  ？？
 int node_num; 			//节点数目
 int edge_num; 			//边数目
 int consume_node_num; 	   //消费节点数目
 int server_deploy_cost; 		//单位服务器部署成本
 int joint_num;    //各蚂蚁路径交汇点数目
 int *crossNodeIds;//对应交汇点ID
 int ants_num;//蚂蚁的数目，初值让其等于消费节点数目
 int stackSizeControll;   //控制BFS搜索宽度

 int BFSSavedTimes;   //？这个参数的作用
time_t StartTime;  //开始时间
time_t StopTime;   //结束时间

 ConsumeNode * ALL_ConsumeNodes;
 Node * ALL_Nodes; 		//所有网络节点
 Edge * ALL_Edges;		//所有边
 Edge *** Edge_Matrix ;		//边的邻接矩阵

 map<int,Ant*>  ants;	//蚂蚁
 list<JointPoint*> ALL_JointPoints;
 list<int> Bad_BFS_Search_Nodes;
 vector<int> CandidateNodes;  //存放好的候选服务器节点
 vector<int>*** ALL_Paths;//存放两两节点之间的费用最短路径
 int bigRoundTime;

 Solution* solution;//最终的解决方案
