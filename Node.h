/*
 * Node.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "Ant.h"


#ifndef NODE_H_
#define NODE_H_

using namespace std;

class Node{

	public:
		list<int> neighbors;
		int type;  //=2说明此节点是交汇点
		int bwNeed;
		list<int> antsPassedBy;
		int antCosts;//记录经过此节点的蚂蚁的总花费，此节点是交汇点的时候才计算

		//forfindRealServer
		list<int> ForFindRealServerList;
		int ForFindRealServer;
		bool ForFindRealServerBool;

		//for kickOut
		bool isKickOuted;

		//for bestCandidateNodes
		int candiCount;

		//for kick out level
		int	kickLevel;

	Node();

	void OnAntPassedBy(int ant_id);
	void AddNeighbor(int nid);
	void DeleteNeighbor(int nid);
	void PrintAntIdsPassedBy();
	list<int> RealAntsCount();
	string ToString();
	void CalculateAntCosts();
};

class ConsumeNode
{
public:
	int net_node_id;
	int bwNeed;
	ConsumeNode()
	{
		net_node_id = 0;
		bwNeed = 0;
	}

	ConsumeNode(int id, int bw)
	{
		net_node_id = id;
		bwNeed = bw;
	}
};

//蚂蚁走下一步的返回结构
struct NextDestination
{
public:
	int lastNodeId;  //前一个网络节点
	list<int> nextNodeIds;//下一个网络节点
	list<int> bandWidths;  //带宽
	int length;//-1代表蚂蚁已经停了，-2表示路径费用大于服务器费用，>=2表示分裂了
};




#endif /* NODE_H_ */
