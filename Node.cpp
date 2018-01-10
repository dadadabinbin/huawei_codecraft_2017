/*
 * Node.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include "ALLSPACE_VARIABLES.h"

	Node::Node()
	{
		type = 0;
		bwNeed = 0;
		antCosts = -1;
		ForFindRealServer = 0;
		ForFindRealServerBool = false;
		isKickOuted = false;
		candiCount = 0;
		kickLevel = 10;//剔除服务器的等级一开始标记为10
	}
//计算到此节点的蚂蚁的总花费
	void Node::CalculateAntCosts()
	{
		int result = 0;
		for(list<int>::iterator it = antsPassedBy.begin(); it != antsPassedBy.end(); it++)
		{
			result	+= find_Ant_In_All_Ants(*it)->currentCost;
		}
		antCosts = result;
	}

	void Node::OnAntPassedBy(int id)
	{
		antsPassedBy.push_back(id);
	}
	void Node::AddNeighbor(int nid)
	{
		neighbors.push_front(nid);
	}

	void Node::DeleteNeighbor(int nid)
	{
		neighbors.remove(nid);
	}
	void Node::PrintAntIdsPassedBy()
	{
		cout<<"passedAntIds : ";
		for(list<int>::iterator it =
			antsPassedBy.begin();
				it != antsPassedBy.end();
				it++)
		{
			cout<<*it << " , ";
		}
		cout <<endl;
	}
    //得到此节点上不同消费节点蚂蚁，返回值是消费节点所在的网络节点
	list<int> Node::RealAntsCount()
	{
		list<int> cons_node_ids;
		for(list<int>::iterator it = antsPassedBy.begin();
			it != antsPassedBy.end();)
		{
			int ant_id = (*it);
			Ant* ant = find_Ant_In_All_Ants(ant_id);
			if(ant == 0)
			{
				it++;
				continue;
			}
			int consId = ant->path[0];
			//设置标志位是为了不让cons_node_ids里面有相同的节点
			 bool alreadyExist = false;
			 for(list<int>::iterator it = cons_node_ids.begin();
					 it != cons_node_ids.end();
					 it++)
			 {
				 if(*it == consId)
				 {
					 alreadyExist = true;
				 }
			 }
			 if(!alreadyExist)
			 {
				 cons_node_ids.push_back(consId);
			 }
				it++;
		}
		return cons_node_ids;
	}

	string Node::ToString()
	{
		ostringstream oStr("neighbors: ");
		list<int>::iterator it = neighbors.begin();
		while(it!= neighbors.end())
		{
			oStr<<*it<<",";
			it++;
		}
		oStr<<" ";
		oStr<<" type:" << type<<" ";
		oStr<<" bwNeed:"<<bwNeed << " ";
		return oStr.str();
	}
