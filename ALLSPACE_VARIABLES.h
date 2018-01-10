/*
 * ALLSPACE_VARIABLES.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#ifndef ALLSPACE_VARIABLES_H_
#define ALLSPACE_VARIABLES_H_
#include "Node.h"
#include "Ant.h"
#include "Edge.h"
#include "JointPoint.h"
#include "Solution.h"
#include "ConsNode_bw_Solution.h"
#include "BFS_KickOut.h"
#include "spfa.h"
#include <map>
#include <list>
extern int bfsNum;
extern int ant_id;
extern int node_num; 			//节点数目
extern int edge_num; 			//边数目
extern int consume_node_num; 	//消费节点数目
extern int server_deploy_cost; 		//单位服务器部署成本
extern int joint_num;    //各蚂蚁路径交汇点数目
extern int *crossNodeIds;//对应网络节点的交汇点次数
extern int ants_num;//蚂蚁的数目，初值让其等于消费节点数目
extern int stackSizeControll;

extern Node * ALL_Nodes; 		//所有网络节点
extern Edge * ALL_Edges;		//所有边
extern ConsumeNode * ALL_ConsumeNodes; //
extern Edge *** Edge_Matrix ;		//边的邻接矩阵

extern map<int,Ant*>  ants;	//蚂蚁
extern list<JointPoint*> ALL_JointPoints;

extern Solution* solution;//最终的解决方案

extern int BFSSavedTimes;//how many time bfs search was saved
extern time_t StartTime;
extern time_t StopTime;
extern list<int> Bad_BFS_Search_Nodes;//for force search
extern vector<int> CandidateNodes;
extern vector<int>*** ALL_Paths;
extern int bigRoundTime;

#endif /* ALLSPACE_VARIABLES_H_ */
