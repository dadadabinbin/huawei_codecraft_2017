/*
 * Server_bw_Solution.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */

#ifndef SERVER_BW_SOLUTION_H_
#define SERVER_BW_SOLUTION_H_
#include <list>
#include <queue>
#include <vector>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>

using namespace std;

class Server_bw_Solution{

public:
		int server_node_id;	//服务器节点id
		int path_num;		//路径数量
		vector<int>  bandWidths;  //占用带宽
		vector<vector<int>>  paths;	//路径
		//int cost;			//带宽成本
		Server_bw_Solution()
		{
	        server_node_id=0;	//服务器节点id
			path_num=0;		//路径数量
		//	cost=0;			//带宽成本
		}

};

#endif /* SERVER_BW_SOLUTION_H_ */
