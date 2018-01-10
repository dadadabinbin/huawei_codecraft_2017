/*
 * spfa.h
 *
 *  Created on: Mar 27, 2017
 *      Author: root
 */
#include <list>

#ifndef SPFA_H_
#define SPFA_H_

using namespace std;
class spfa
	{
public:
		vector<int> path;
		int bandWidth;
		int fee;
	};


BFSzy SPFA(int position, int bwNeed, list<int> serverIds, BFSzy& bz);

vector<int> findCandidateNodesWithSPFA();
#endif /* SPFA_H_ */
