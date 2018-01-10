/*
 * Ant.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#ifndef ANT_H_
#define ANT_H_
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;


class Ant
{
public:
	int id;
	int currentPosition;//当前位置
	vector<int> path;
	list<int> abandonedNodes; //考虑过的节点
	int currentCost;
	int bandWidth;
	bool isStoped;
	int stoppedReason;// -2 : cost too much, -3: no way to go

	Ant();
	Ant(int pos, int bandW);
	~Ant();
	int CalculateCost();
	string ToString();
	void OnNoWayToGo();
	void OnCrossed(int crossedNodeId);
};

Ant* find_Ant_In_All_Ants(int antId);

#endif /* ANT_H_ */
