/*
 * BFS_KickOut.h
 *
 *  Created on: Mar 21, 2017
 *      Author: root
 */

#ifndef BFS_KICKOUT_H_
#define BFS_KICKOUT_H_
#include <list>
#include <queue>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>

using namespace std;
//BFS和SPFA专用数据结构
//这个结构是从一个消费节点所在的节点出发，到其他服务器节点的费用最短路径
class BFSzy
{
public:
	vector< vector<int> > mulPath;
	int cost;
	vector<int> bandWidth;
	int currentPosition;

	BFSzy()
	{
		cost = 0;
		currentPosition = 0;
	};

	BFSzy(int startId, int bw)
	{
		cost = 0;
		currentPosition = startId;
		bandWidth.push_back(bw);
		vector<int> * tmp = new vector<int>();
		mulPath.push_back(*tmp);
		delete tmp;
		mulPath[0].push_back(startId);
	};

	string ToString()
	{
		ostringstream oStr("");
		oStr<<"		cost: " <<cost <<endl;

		oStr<<"		paths :"<<endl;
		for(int i = 0; i < mulPath.size(); i++)
		{
			oStr<<"			"<<i<<":";
			vector<int> tmpP = mulPath[i];
			for(int j = 0; j < tmpP.size(); j++)
			{
				oStr<<tmpP[j] << "  ";
			}
			oStr<<endl;
			oStr<<"		bandWidth: " << bandWidth[i] <<endl;
		}
		return oStr.str();
	}
};
//用于bfs搜索的蚂蚁
class bfs
	{
	public:
		int now_p;
		int bandWidth;
		int cost;
		int isStop;
		vector<int> path;
	};
//bfs搜索的邻居节点
class bfs_neighbor
{
public:
	int id;
	int fee;
	int left;
};
//广度优先搜索加剪枝
BFSzy BFSsearch(BFSzy& z);

#endif /* BFS_KICKOUT_H_ */
