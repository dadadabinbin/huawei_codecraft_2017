/*
 * Edge.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <string>
using namespace std;

class Edge{
public:
	int startNodeId;
	int endNodeId;
	int _bandWidth;
	int fee;
	int Upleft;  //上行带宽  //显示实时流量
	int Downleft;//下行带宽
	int m_Upleft;  //作为记忆保存之前的流量
	int m_Downleft;

	Edge();
	Edge(int start, int end, int bandW, int fe);
	string ToString();
	int GetLeft(int start, int end);
	bool RequireForBW(int start, int end, int bw);
	void GiveBackBW(int start, int end, int bw);
	void ReInitBandWidth();
	void GenerateMemory();
	void RestoreToLastMemory();
};



#endif /* EDGE_H_ */
