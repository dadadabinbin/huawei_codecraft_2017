/*
 * ConsNode_bw_Solution.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#ifndef CONSNODE_BW_SOLUTION_H_
#define CONSNODE_BW_SOLUTION_H_
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "Edge.h"

using namespace std;


class ConsNode_bw_solution
{
public:
	int cons_node_id;
	int path_num;
	list<int>* paths;
	int* bws;
	int cost;
	bool isSatisfied;//1 nice, 2 deploy at local buf satisfied, 0 not satsified
	bool Initialized;  //标记是否初始化成功，即此类中的变量是否都已经赋值

	ConsNode_bw_solution();
	int CostCalculate();
	bool TakeBandWidths();
	void GiveBackBandWidths();
	string ToString();
};



#endif /* CONSNODE_BW_SOLUTION_H_ */
