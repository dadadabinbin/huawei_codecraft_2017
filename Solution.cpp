/*
 * Solution.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include "ALLSPACE_VARIABLES.h";

Solution::Solution()
{
		bwSolution = 0;
		cost = 0;
}

int Solution::CalculateCost()
{
		int result = 0;
		result += serverIds.size() * server_deploy_cost;
		for(int i = 0 ; i < consume_node_num; i++)
		{
//			cout<<"bw_solution cost count i: "<< i<<endl;
			bwSolution[i].cost = bwSolution[i].CostCalculate();
//			cout<<"bw_solution cost count finished"<<endl;
			result += bwSolution[i].cost;
		}
		cost = result;
		return cost;
}

bool Contains(list<int> _list, int value);
bool CheckIfTwoListEqual(list<int>* a, list<int>* b);

//删除重复的路径
ConsNode_bw_solution* DeleteRepeatedPath( ConsNode_bw_solution* cons)
{
	cout<<"enter deletedReapetead Path in " << cons->cons_node_id  << " path_num " << cons->path_num<< endl;
	//重新构造删除重复路径后的ConsNode_bw_solution
	int _path_num = 0;
	vector<list<int> *> realPaths;
	vector<int> realBws;

	if(cons->path_num > 0)
	{
		for(int i = 0; i < cons->path_num; i++)
		{
			if(realPaths.size() == 0)//blank, add Path//为空增加路径
			{
				cout<<"realPath blank, initialize"<<endl;
				realPaths.push_back(&cons->paths[i]);
				realBws.push_back(cons->bws[i]);
				_path_num++;
			}
			else{

				cout<<"check " << i <<"th path, _path_num " << _path_num <<endl;
				bool checkToAdd = true;//标志位是为了不增加重复的路径
				for(int j = 0; j < _path_num; j++)
				{
					list<int>* realPath = realPaths[j];
					if(CheckIfTwoListEqual(realPath, &cons->paths[i]))//repeated path ,add bws
					{
						cout<<"find repeated path , realBws[j] "<< realBws[j] << " cons->bws[i] " << cons->bws[i]<<endl;
						realBws[j] += cons->bws[i];
						checkToAdd = false;
					}
				}

				if(checkToAdd)//not repeated, add to the end
				{
					cout<<"find new path";
					realPaths.push_back(&cons->paths[i]);
					realBws.push_back(cons->bws[i]);
					_path_num++;
				}

			}
		}

	}
    //tempPaths起到临时存放的作用
	list<int>* tempPaths = new list<int>[_path_num];
	for(int i = 0; i < _path_num; i++)
	{
		tempPaths[i] = *realPaths[i];
	}

	//refresh infomation in cons_bw_solution
	cons->path_num = _path_num;
	for(int i = 0; i < _path_num;i++)
	{
		cons->paths[i] = tempPaths[i];
		cons->bws[i] = realBws[i];
 	}
	delete [] tempPaths;

	cout<<"leave deletedReapetead Path in " << cons->cons_node_id << endl;
	return cons;
}
//检查两条路径是否完全相同
bool CheckIfTwoListEqual(list<int>* a, list<int>* b)
{
	if(a->size() == b->size())
	{
		list<int>::iterator a_iter = a->begin();
		list<int>::iterator b_iter = b->begin();
		for(;a_iter != a->end(); a_iter++,b_iter++)
		{
			if(*a_iter != *b_iter)
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
//得到最好的参考服务器
void Solution::GetBestCandidateServers()
{
	cout<<"Enter get_best_candidate servers" << endl;
	//先把所有节点的type标记为1，也就是将之前交互点type=2的情况去掉
	for(int i = 0; i < node_num; i++)
	{
		ALL_Nodes[i].type = 1;
	}
	//find cross_nodes connected with more than two consumeNodes
	list<int> result;
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution* cons = bwSolution + i;
		//delete RepeatedPath in one cons_bw_solution
		cons = DeleteRepeatedPath(cons);

		for(int j =0 ; j < cons->path_num; j++)//+1 for each path passed through cross_node l.back()
		{
			list<int> l = cons->paths[j];
			ALL_Nodes[l.back()].ForFindRealServer++;//路径最后节点
		}
        //初始情况消费节点满足情况设置为false
		cons->isSatisfied = false;//Set all to false
	}

	//get best candidate servers
	for(list<int>::iterator it = serverIds.begin();
			it != serverIds.end(); it++)
	{
		//如果这个服务器供应了两个和两个以上消费节点，那么这就是一个好的消费节点
		if(ALL_Nodes[*it].ForFindRealServer >= 2)//nice candidate, add to result
		{
			result.push_back(*it);//result放入连接两个消费节点的服务器节点
			ALL_Nodes[*it].type = 2;
		}
		else//bad candiate//如果这个服务器供应了一个消费节点，那么这就是一个坏的消费节点
		{
			//将所有消费节点出发经过这个服务器节点的路径花费都重置为0
			for(int i = 0; i < consume_node_num; i++)//set all consume nodes passed through this cross_node, path_num = 0
			{
				ConsNode_bw_solution* cons = bwSolution + i;
				for(int j = 0; j < cons->path_num; j++)
				{
					if(cons->paths[j].back() == *it)
					{
						cons->cost = 0;
						cons->path_num = 0;
						break;
					}
				}
			}
		}
	}
	//对各个消费节点检查流量情况，看是否被满足
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution * cons = bwSolution + i;
		//check if consumeNode is currently satisfied
		int bwInAllPath = 0;
		for(int j = 0; j < cons->path_num;j++)
		{
			bwInAllPath += cons->bws[j];
		}
		if(bwInAllPath == ALL_ConsumeNodes[cons->cons_node_id].bwNeed)//consume node is satisfied
		{
			cout<<"find a original best solution in cons_node " << cons->cons_node_id<<endl;
			cout<<"bwInAllPath: "<<bwInAllPath << " bwNeed " << ALL_ConsumeNodes[cons->cons_node_id].bwNeed<<endl;
			cons->isSatisfied = true;
		}
	}
     //此时的serverId存放的是供应两个及以上的消费节点的服务器节点
	serverIds = result;

	//joint_num currently not used?
	//重置joint_num的大小，等于供应两个及以上的消费节点的服务器节点的数目
	 joint_num = 0;
	 for(list<int>::iterator it = serverIds.begin(); it != serverIds.end(); it++, joint_num++)
	 {
		   crossNodeIds[joint_num] = *it;
	 }
	 joint_num = serverIds.size();

	 cout<<"leave get_best_candidate servers" << endl;
	//gengxin quan ju xinxi
}

bool Solution::IsAllNodeSatisfied()
{
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution* cons = bwSolution + i;
		if(!cons->isSatisfied)
			return false;
	}
	return true;
}

//得到真实的服务器
void Solution::GetRealServers()
{
	list<int> result;

	//reinitial set all to the beginning
	//重置所有节点
	for(list<int>::iterator it = serverIds.begin();
			it != serverIds.end(); it++)
	{
		ALL_Nodes[*it].ForFindRealServer = 0;
		ALL_Nodes[*it].ForFindRealServerList.clear();
		ALL_Nodes[*it].ForFindRealServerBool = false;
		ALL_Nodes[*it].type = 1;
	}

	//server passed by count
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution* cons = bwSolution + i;
		//服务器直连
		if(cons->path_num == 1 && cons->paths[0].size() ==1)//the server deployed next to consume_node
		{
				ALL_Nodes[cons->paths[0].back()].ForFindRealServerBool = true;
		}else{
			for(int j = 0; j < cons->path_num; j++)//the server reached by passes
			{
				if(!Contains(ALL_Nodes[cons->paths[j].back()].ForFindRealServerList, i))
					ALL_Nodes[cons->paths[j].back()].ForFindRealServerList.push_back(i);
				ALL_Nodes[cons->paths[j].back()].ForFindRealServer += 1;
			}
		}
	}

	//get new servers
	//保留好的服务器，删除差的服务器
	for(list<int>::iterator it = serverIds.begin();	it != serverIds.end(); it++)
	{
			joint_num = 0;

			if(ALL_Nodes[*it].ForFindRealServerBool)
			{
				result.push_back(*it);
				crossNodeIds[joint_num++] = *it;
				ALL_Nodes[*it].type = 2;
			}
			else if(ALL_Nodes[*it].ForFindRealServer >= 2 && ALL_Nodes[*it].ForFindRealServerList.size() >=2 )
			{
				result.push_back(*it);
				crossNodeIds[joint_num++] = *it;
				ALL_Nodes[*it].type = 2;
			}
			else//delete server//删除差的服务器，流量回退
			{
				cout<<"server "<< *it <<" is deleted "<<endl;
				for(int i = 0; i < consume_node_num; i++)
				{
					ConsNode_bw_solution* cons = bwSolution + i;
					for(int j = 0; j < cons->path_num; j++)
					{
						if(cons->paths[j].back() == *it)
						{
							cout<<"set consume_node" << cons->cons_node_id << "as unsatisfied because server "<< *it << " is deleted"<<endl;
							cons->GiveBackBandWidths();
							cons->isSatisfied = false;
							cons->cost = 0;
							cons->path_num = 0;
							break;
						}
					}
				}
			}
	}
	serverIds = result;
}
string Solution::ToString()
{
	ostringstream oStr("solution: ");
    oStr << "cost: " << this->cost;
    oStr<<", server num "<<serverIds.size();
	oStr<<endl;
	oStr<<"servers :"<<endl;
    for(list<int>::iterator it=serverIds.begin();it!=serverIds.end();it++)
    {
    	oStr<<" "<<*it<<" ";
    }
    for(int i=0;i < consume_node_num;i++)
    {
    	oStr<<bwSolution[i].ToString();
    }
	return oStr.str();
}
void ReInitAllBandWidths();
//检查解决方案的有效性
bool Solution::CheckIfSolutionEffective()
{
	ReInitAllBandWidths();
	//check bandWidth
	for(int i = 0; i < consume_node_num; i++)
	{
		cout<<"=============================================="<<endl;
		cout<<"check consume node solution " << i << endl;
		int bandW = 0;
		ConsNode_bw_solution *cons = bwSolution + i;
		//检查流量是否满足
		for(int j = 0; j < cons->path_num; j++)
		{
			cout << "check path " << j << endl;
			list<int>::iterator it2 = cons->paths[j].begin();
			it2++;
			bandW += cons->bws[j];
			for(list<int>::iterator it = cons->paths[j].begin();
					it2 != cons->paths[j].end(); it++, it2++)
			{
				Edge* edge = Edge_Matrix[*it][*it2];
				if(*it == *it2)
				{
					cout<<"check Exception whatt fuck?"<<endl;
				}
				if(!edge->RequireForBW(*it, *it2, cons->bws[j]))
				{
					return false;
				}
			}
		}
		if(bandW != ALL_ConsumeNodes[cons->cons_node_id].bwNeed)
		{
		cout<<"consnode id " << cons->cons_node_id <<" bandWidth not equal with Need, need: "<<ALL_ConsumeNodes[cons->cons_node_id].bwNeed;
		cout<<", infact: "<< bandW <<endl;
		return false;
		}
	}
	return true;
}

//int Solution::GetWorstServerId()
//{
//	float WorstCost = 0;
//	int id = 0;
//
//	for(list<int>::iterator it = serverIds.begin(); it != serverIds.end(); it++)
//	{
//		int bandWidth = 0;
//		float leastCost = server_deploy_cost;
//
//		for(int i = 0; i < consume_node_num; i++)
//		{
//			ConsNode_bw_solution* cons = bwSolution + i;
//			for(int j = 0; j < cons->path_num; j++)
//			{
//				list<int>* _path = &cons->paths[j];
//				if( _path->back() == *it)
//				{
//					bandWidth += cons->bws[j];
//					list<int>::iterator it2 = _path->begin();
//					it2++;
//					for(list<int>::iterator it1 = _path->begin(); it2 != _path->end(); it1++, it2++)
//					{
//						leastCost += cons->bws[j] * Edge_Matrix[*it1][*it2]->fee;
//					}
//				}
//			}
//			if(cons->path_num == 0)
//			{
//				cout<<"path_num = 0 ???????"<<endl;
//				string str;
//				cin>>str;
//			}
//			if(cons->path_num == 1 && cons->paths[0].size() == 1 && cons->paths[0].back() == *it)
//			{
//					bandWidth += ALL_ConsumeNodes[cons->cons_node_id].bwNeed;
//			}
//		}
//
//		if(bandWidth == 0)
//		{
//				cout<<"??????????????"<<endl;
//				string str;
//				cin>>str;
//		}
//		leastCost = leastCost/bandWidth;
//
//		if(leastCost > WorstCost && ALL_Nodes[*it].isKickOuted != true)
//		{
//			WorstCost = leastCost;
//			id = *it;
//			cout << "Get worst ServerId " << id << endl;
//			ALL_Nodes[*it].isKickOuted = true;
//		}
//	}
//	return id;
//}
//计算服务器的性价比，其中性价比=总费用（服务器费用加路径费用）除以带宽，性价比最低的为一开始就要剔除的服务器
int Solution::GetWorstServerId()
{
	float WorstCost = 0;
	int id = 0;
	cout<<"==========================Get in Get worst ServerId"<<endl;
	for(list<int>::iterator it = serverIds.begin(); it != serverIds.end(); it++)
	{
		int bandWidth = 0;
		float leastCost = server_deploy_cost;

		for(int i = 0; i < consume_node_num; i++)
		{
			ConsNode_bw_solution* cons = bwSolution + i;
			for(int j = 0; j < cons->path_num; j++)
			{
				list<int>* _path = &cons->paths[j];
				if( _path->back() == *it)
				{
					bandWidth += cons->bws[j];
					list<int>::iterator it2 = _path->begin();
					it2++;
					for(list<int>::iterator it1 = _path->begin(); it2 != _path->end(); it1++, it2++)
					{
						leastCost += cons->bws[j] * Edge_Matrix[*it1][*it2]->fee;
					}
				}
			}
			//leastCost = leastCost/bandWidth;
			leastCost = leastCost/bandWidth;
		}

		if(leastCost > WorstCost && ALL_Nodes[*it].isKickOuted != true)
		{
			WorstCost = leastCost;
			id = *it;
			ALL_Nodes[*it].isKickOuted = true;
		}
	}
	cout << "Get worst ServerId " << id << endl;
	cout<<"==========================leave Get worst ServerId"<<endl;
	return id;
}

//根据性价比踢服务器
void Solution::KickOutWorstServer( int id)
{
	ALL_Nodes[id].type = 1;//将剔除的点类型恢复成普通点
	ALL_Nodes[id].kickLevel--;
	serverIds.remove(id);//服务器列表中剔除该点
	
	//对该消费节点的路径流量花费等进行回退，恢复成初始状态
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution* cons = bwSolution + i;
		for(int j = 0; j < cons->path_num; j++)
		{
			list<int>* _path = &cons->paths[j];
			if( _path->back() == id)
			{
				cons->GiveBackBandWidths();
				cons->isSatisfied = false;
				cons->path_num = 0;
				cons->cost = 0;
				break;
			}
		}
	}
}
//将vector<int> ids



//服务器都kickout一遍
void Solution::BigRoundKickOut(vector<int> ids)
{
	for(vector<int>::iterator it = ids.begin(); it != ids.end(); it++){
		int id = *it;
		ALL_Nodes[id].type = 1;
		//ALL_Nodes[id].kickLevel--;
		serverIds.remove(id);

		for(int i = 0; i < consume_node_num; i++)
		{
			ConsNode_bw_solution* cons = bwSolution + i;
			for(int j = 0; j < cons->path_num; j++)
			{
				list<int>* _path = &cons->paths[j];
				if( _path->back() == id)
				{
					cons->GiveBackBandWidths();
					cons->isSatisfied = false;
					cons->path_num = 0;
					cons->cost = 0;
					break;
				}
			}
		}
	}
}
//发现不在参考点的服务器
vector<int> Solution::FindServerIdNotInCandidate()
{
	vector<int> result;
	vector<int> realResult;
	//发现不在参考点的服务器节点
	for(list<int>::iterator it = serverIds.begin();  it != serverIds.end(); it++)
	{
		bool contains = false;
		for(int i = 0 ; i < CandidateNodes.size(); i++)
		{
			if(CandidateNodes[i] == *it)
			{
				contains = true;
				break;
			}
		}
		if(!contains)
		{
			result.push_back(*it);
		}
	}
    //此服务器节点也不能是消费节点直连的服务器节点
	for(int i = 0 ; i < result.size(); i++)
	{
		bool add = true;
		for(int j = 0; j < consume_node_num; j++)
		{
			if(ALL_ConsumeNodes[j].net_node_id == result[i])
			{
				add = false;
			}
		}
		if(add)
		{
			realResult.push_back(result[i]);
		}
	}

#if 0
	cout<<"server not in candidates: ";
	for(int i = 0 ; i < realResult.size(); i++)
	{
		cout<<realResult[i]<<" ";
	}
	cout<<endl;

#endif

	return realResult;
}

//得到真正好的服务器
void Solution::GetRealNiceServers()
{
	for(list<int>::iterator it = serverIds.begin(); it != serverIds.end(); it++)
	{
			ALL_Nodes[*it].isKickOuted = false;
	}

	vector<int> kickOutedIds;
	//addNum记录初始serverids的大小
	int addNum = serverIds.size();
//	int roundTime = serverIds.size() - (2+bigRoundTime) ;
//	if(roundTime <= 0)
//	{
//		roundTime = 5;
//	}

//	for(int i = 0; i < roundTime; i++)
//	{
//		int id = GetWorstServerId();
//		kickOutedIds.push_back(id);
//	}
	for(list<int>::iterator it = serverIds.begin(); it != serverIds.end(); it++)
	{
		ALL_Nodes[*it].kickLevel += 1;
		kickOutedIds.push_back(*it);
	}

	BigRoundKickOut(kickOutedIds);

	for(vector<int>::iterator it = kickOutedIds.begin(); it != kickOutedIds.end(); it ++)
	{
		ALL_Nodes[*it].isKickOuted = false;
	}

	int _i = addNum - 1;
	AddCandidateServers(_i);
}
bool LevelCompare(int a, int b);

//根据kickLevel的大小来增加服务器
void Solution::AddCandidateServers(int serverTotalNum)
{
	int addNum = serverTotalNum;
	list<int> allNodeIds;
	for(int i = 0; i < node_num;i++)
	{
		allNodeIds.push_back(i);
	}
	allNodeIds.sort(LevelCompare);

//	std::sort(CandidateNodes.begin(), CandidateNodes.end(), LevelCompare);
	for(int i = 0 ; i < addNum; i++)
	{
		int candidateId = allNodeIds.front();
		allNodeIds.pop_front();
		serverIds.push_back(candidateId);
		ALL_Nodes[candidateId].type = 2;
		cout<<"add server : "<<candidateId<<endl;
	}

//	cout<<"leave add candidate server"<<endl;
}

void Solution::SolutionCopy(Solution* _solution)
{
	cout<<"enter solution copy"<<endl;
	serverIds = _solution->serverIds;
	cost = _solution->cost;
	string str;
	cout<<"consume_node_num "<<consume_node_num<<endl;
	for(int i = 0 ; i < consume_node_num; i++)
	{
		ConsNode_bw_solution *cons = bwSolution + i;
		ConsNode_bw_solution *_cons = _solution->bwSolution + i;

		cons->cons_node_id = _cons->cons_node_id;
		if(cons->path_num > 0)
		{
			delete [] cons->paths;
			delete [] cons->bws;
		}
		//cin>>str;
		cons->path_num = _cons->path_num;
		cons->paths = new list<int>[cons->path_num];
		cons->bws = new int[cons->path_num];
		for(int j = 0 ; j < cons->path_num; j++)
		{
			cons->paths[j] = _cons->paths[j];
			cons->bws[j] = _cons->bws[j];
		}
	}
	cout<<"leave solution copy"<<endl;
}
