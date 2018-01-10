/*
 * Ant.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#include "ALLSPACE_VARIABLES.h"
#include "algorithm"
using namespace std;
Ant::Ant()
	{
		id = ++ant_id;
		currentPosition = 0;
		currentCost = 0;
		bandWidth = 0;
		isStoped = false;
		stoppedReason = -1;
	}
	Ant::Ant(int pos, int bandW)
	{
		id = ++ant_id;
		currentPosition = pos;
		path.push_back(pos);
		abandonedNodes.push_front(pos);
		 currentCost = 0;
		 isStoped = false;
		 bandWidth = bandW;
		//	cout<<"==========ant created antId: "<<id << endl;
		 stoppedReason = -1;
	}

	Ant::~Ant()
	{
		if(path.size() >0)
		{
			Ant* ant = this;
			for(int i = 0 ; i < path.size(); i++)
			{
				for(list<int>::iterator it = ALL_Nodes[path[i]].antsPassedBy.begin();
						it != ALL_Nodes[path[i]].antsPassedBy.end();)
				{
					if((*it) == ant->id)
					{
						it = ALL_Nodes[path[i]].antsPassedBy.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
		}
	}
//蚂蚁无路可走，走到最后流量走不过去了
	void Ant::OnNoWayToGo()
	{
		isStoped = true;
		stoppedReason  = -3;
		//流量回退
		if(path.size() >= 2)
		{
			for(int i = 0 ; i < path.size() - 1; i++)
			{
				Edge_Matrix[path[i]][path[i+1]]->GiveBackBW(path[i], path[i+1], bandWidth);
			}
		}
	}
//到id时候分出多条路，其中一条路走到最后带宽需求超过路径要求，则流量回退
	void Ant::OnCrossed(int id)
	{
		isStoped = true;
		stoppedReason = -2;
		for(vector<int>::iterator i = path.begin(); i != path.end() ;)
		{
			if(*i == id)
			{
				for(; i != path.end(); i++)
				{
					Edge_Matrix[*i][*i+1]->GiveBackBW((*i), *(i+1) , bandWidth);
				}
			}
		}
	}


	int Ant::CalculateCost()
	{
		int result = 0;
		for(int j = 0; j < path.size() - 1; j++)
		{
			Edge* edge = Edge_Matrix[ path[j] ][ path[j + 1] ];
			result += edge->fee * bandWidth;
		//	cout<<"check edge " <<  path[j] << " " << path[j + 1] << ", cost : " << edge.fee<<endl;
		}
		currentCost = result;
		return result;
	}

	string Ant::ToString()
	{
		ostringstream oStr("ant");
		oStr<<endl<< "AntId: " << id <<endl<<"path: ";
		for(int i = 0; i < path.size(); i++)
		{
			oStr<<path[i] << " , ";
		}

		oStr <<endl<<"bandWidth: " << bandWidth <<endl;
		oStr <<"currentCost : "<< currentCost<< endl;
		oStr <<"stoppedReason : "<< stoppedReason <<endl;
		oStr << "isStopped: " << isStoped << endl;

		return oStr.str();
	}

	Ant* find_Ant_In_All_Ants(int antId)
	{
		return ants[antId];
	}


