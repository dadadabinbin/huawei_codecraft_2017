/*
 * Solution.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "ConsNode_bw_Solution.h"

using namespace std;


class Solution{
public:
	list<int> serverIds;
	ConsNode_bw_solution* bwSolution;
	int cost;
	Solution();
	int CalculateCost();
	void GetBestCandidateServers();
	void GetRealServers();
	bool IsAllNodeSatisfied();
	string ToString();
	bool CheckIfSolutionEffective();
	int GetWorstServerId();

	void KickOutWorstServer(int id);
	vector<int> FindServerIdNotInCandidate();
	void GetRealNiceServers();
	void AddCandidateServers(int serverTotalNum);
	void BigRoundKickOut(vector<int> ids);
	void SolutionCopy(Solution* solution);
};



#endif /* SOLUTION_H_ */
