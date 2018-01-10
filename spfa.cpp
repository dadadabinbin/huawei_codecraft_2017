#include "ALLSPACE_VARIABLES.h"

//BFSzy* GenerateConsSolution(spfa* minn, int bwNeed,list<int> serverIds, BFSzy& bz, int position);
/*
  改进的spfa分析：
  难点：在找到费用最小的路径的同时，要考虑服务器的位置，以及上下行流量的控制
  这个上下行流量设计成实时变化的，从而达到流量满足的目的
  思路分析：
  从一个消费节点出发
     1 首先我们要定义两个变量，一个是消费节点的需求totalNeed和成本cost,利用这两个变量来控制循环的终止条件，
	 totalNeed一直更新，直到带宽被每次的最小费用路径的带宽分担到为0，或者总成本增加到直连服务器费用成本以上
	 2 我们的返回结构为BFSzy,里面包括当前消费节点所在的服务器节点，和该消费节点到所有服务器节点的费用最小路径
	 以及每条路径的带宽花费，还有总成本
	 3 每次大循环的目标是求出到消费节点到一个服务器节点的成本最小路径
	 4 在大循环中建立一个大小为网络节点的数组，里面存放spfa结构，包括消费节点到该节点的费用，带宽和路径
	 5 建立一个队列，这里有一个小循环，直到这个队列为空的时候跳出循环
	 6 在小循环中，队列先弹出头部元素，然后对头部元素（网络节点）的邻居节点进行遍历，如果发现当前节点的费用
	 加上边的费用比邻居节点的费用还要小，则更新邻居节点，并将邻居节点入队列，中间注意流量控制
	 7 经过前面的小循环，可以找到消费节点到所有网络节点的最小费用路径，根据服务器节点所在网络节点的位置，找到
	 其中费用最小的一条路径，放入返回结构中
	 8 更新相关路径的带宽，（扣除掉这次路径的相关带宽分担）更新totalNeed和cost,进入下一次循环
	 9 spfa可以理解为根据队列结果对一个表进行实时更新

*/

//改进的spfa
/*
*@paramter:服务器节点位置，消费节点位置和需求，建立消费节点到所有点的最短路径
*
*/
BFSzy SPFA(int position, int bwNeed, list<int> serverIds, BFSzy& bz)
{

//	cout<<"enter SPFA"<<endl;
    //初始totalNeed记录该消费节点的带宽需求，之后各路径依次分此带宽需求
	int totalNeed = bz.bandWidth[0];
	//cost初始为0，表示总费用成本，总费用成本不能超过直连成本，即服务器成本
	int cost = 0;
	//定义返回结构
	BFSzy r_bz;
    //每次大循环的目标是求出到消费节点到一个服务器节点的成本最小路径
	while(totalNeed > 0 && cost < server_deploy_cost)
	{
//		cout<<"+++++++++++++++++++"<<endl;
//		cout<<"totalNeed:"<<totalNeed<<endl;
//		cout<<"cost:"<<cost<<endl;
//		cout<<"server_deploy_cost:"<<server_deploy_cost<<endl;

		//建立一个数组，数组里面存放所求节点到网络中所有节点的费用，带宽和路径
		spfa* minn = new spfa[node_num];
                //初始费用设为无穷大
		for(int i=0;i < node_num; i++)
		{
			minn[i].fee = 9999;
		}
		minn[position].fee = 0;
		minn[position].path.push_back(position);
		//初始该节点带宽设为无穷大
		minn[position].bandWidth = 9999;
        
		//建立一个队列，里面存放可以刷新最短路径的节点，直到不能一直刷新到队列为空
		//先将该节点放入队列中
		deque<int> stack;
		stack.push_back(position);


		while(stack.size() != 0)
		{

			//先取出该节点
			int target = stack.front();
			stack.pop_front();
			//cout<<"stack_size: "<<stack.size()<<endl;
			//cout<<"target:"<<target<<endl;
			
            //该数组存放该节点的邻居节点
			vector<bfs_neighbor> nnn;
			//将邻居节点信息放入数组中
			list<int>::iterator it = ALL_Nodes[target].neighbors.begin();
			while(it != ALL_Nodes[target].neighbors.end())
			{
				//cout<<*it<<endl;
                 //如果该节点费用加上路径费用小于之前该节点费用，则可以刷新
				if(minn[target].fee + Edge_Matrix[target][*it]->fee < minn[*it].fee)
				{
					//此部分为流量控制部分，注意这里路径的流量是实时变化的，所以会出现流量为0的情况
                    //如果路径流量已经为0，则跳过此邻居节点，判断下一个节点
					if(Edge_Matrix[target][*it]->GetLeft(target,*it) == 0)
					{
						it++;
						continue;
					}
                    //如果需求流量大于路径的流量，则将邻居节点流量就设置为路径流量
					if(minn[target].bandWidth > Edge_Matrix[target][*it]->GetLeft(target, *it))
					{
						
						minn[*it].bandWidth = Edge_Matrix[target][*it]->GetLeft(target, *it);

					}
					//如果需求流量小于路径流量，则将下一节点流量改为次节点流量
					else
						minn[*it].bandWidth = minn[target].bandWidth;

					//cout<<"!!!"<<endl;
					//可以刷新，就将该节点放入队列
					stack.push_back(*it);
					//新节点费用设置为原来节点费用加路径费用
					minn[*it].fee = 9999;
					minn[*it].fee = minn[target].fee + Edge_Matrix[target][*it]->fee;
                    //新节点路径是原来路径加上该节点
					minn[*it].path.clear();
					minn[*it].path = minn[target].path;
					minn[*it].path.push_back(*it);


				}
				//cout<<"???"<<endl;
				it++;
			}

		}

		//cout<<"1=========================="<<endl;
        //考虑服务器因素，设置初始服务器节点带宽，费用，节点和路径
		int min_bandWidth = 0;
		int min_fee = 9999;
		int min_serverId = 0;
		vector<int> min_path;
		//将之前spfa计算的最短路径和其他信息设置为服务器信息，找出距离该节点费用最小的服务器
		for(list<int>::iterator it = serverIds.begin(); it != serverIds.end();it++)
		{
			//cout<<"minn[serverId].fee: "<< minn[*it].fee<<endl;
			if(minn[*it].fee < min_fee)
			{
				min_fee = minn[*it].fee;
				min_serverId = *it;
				min_path = minn[*it].path;
				min_bandWidth = minn[*it].bandWidth;
			}
		}
        //如果没有最小费用，则直接返回
		if(min_fee == 9999)
		{
			r_bz.mulPath.clear();
			r_bz.bandWidth.clear();
			r_bz.cost = 65534;
			return r_bz;
		}
        //构造返回结构，其多条路径中放入的是费用最短的服务器路径
		r_bz.mulPath.push_back(min_path);

	    //更新totalNeed带宽需求，直到其为0
		if(totalNeed > min_bandWidth)
		{
			//更新带宽需求
			totalNeed = totalNeed - min_bandWidth;
			//更新费用
			cost = cost + min_bandWidth * min_fee;
			//返回结构加入带宽
			r_bz.bandWidth.push_back(min_bandWidth);
			//cout<<"2=========================="<<endl;
			cout<<"path: ";
			//for(int i=0;i<min_path.size();i++)
			//{
			//	cout<<min_path[i]<< " ";
			//}
			//cout<<endl;
			//更新每条边的上下行带宽
			for(int i=0;i<min_path.size()-1;i++)
			{
				//cout<<"b_Edge_Matrix["<<min_path[i]<<"]["<<min_path[i+1]<<"].left:"<<Edge_Matrix[min_path[i]][min_path[i+1]]->GetLeft(min_path[i],min_path[i+1])<<endl;
				Edge_Matrix[min_path[i]][min_path[i+1]]->RequireForBW(min_path[i],min_path[i+1],min_bandWidth);
				//cout<<"n_Edge_Matrix["<<min_path[i]<<"]["<<min_path[i+1]<<"].left:"<<Edge_Matrix[min_path[i]][min_path[i+1]]->GetLeft(min_path[i],min_path[i+1])<<endl;
			}


		}
		else
		{
			cost = cost + min_fee * totalNeed;
			r_bz.bandWidth.push_back(totalNeed);
			//cout<<"3=========================="<<endl;
			for(int i=0;i<min_path.size()-1;i++)
			{
				//cout<<"b_Edge_Matrix["<<min_path[i]<<"]["<<min_path[i+1]<<"].left:"<<Edge_Matrix[min_path[i]][min_path[i+1]]->GetLeft(min_path[i],min_path[i+1])<<endl;
				Edge_Matrix[min_path[i]][min_path[i+1]]->RequireForBW(min_path[i],min_path[i+1],totalNeed);
				//cout<<"n_Edge_Matrix["<<min_path[i]<<"]["<<min_path[i+1]<<"].left:"<<Edge_Matrix[min_path[i]][min_path[i+1]]->GetLeft(min_path[i],min_path[i+1])<<endl;
			}
			totalNeed = 0;
		}
		delete [] minn;



	}
    //更新返回结构的成本
	r_bz.cost = cost;
    //如果成本大于服务器直接部署费用，则cost为65534，即报错
	if(cost > server_deploy_cost)
	{
		r_bz.mulPath.clear();
		r_bz.bandWidth.clear();
		r_bz.cost = 65534;
	}


	return r_bz;
}
//用来找最佳服务器节点的spfa方法，
/*
*return: 返回该节点到所有网络节点的费用最小路径
*/
spfa* SPFAForCandidateServer(int position, BFSzy& bz)
{

	cout<<"enter SPFAForCandidateServer"<<endl;
    
	int cost = 0;
	//构造数组存放消费节点到其他所有网络节点的路径，带宽，费用
	spfa* minn = new spfa[node_num];

	for(int i=0;i < node_num; i++)
	{
		minn[i].fee = 9999;
	}
	minn[position].fee = 0;
	minn[position].path.push_back(position);
	minn[position].bandWidth = 9999;
	//构造队列
	deque<int> stack;
	stack.push_back(position);

	while(stack.size() != 0)
	{
		//string sss;
		//cin>>sss;
		int target = stack.front();
		stack.pop_front();
		//cout<<"stack_size: "<<stack.size()<<endl;
		//cout<<"target:"<<target<<endl;
		vector<bfs_neighbor> nnn;
		list<int>::iterator it = ALL_Nodes[target].neighbors.begin();
		while(it != ALL_Nodes[target].neighbors.end())
		{
			//cout<<*it<<endl;

			//cout<<"fee:"<<  minn[*it].fee<<endl;
			if(minn[target].fee + Edge_Matrix[target][*it]->fee < minn[*it].fee)
			{
			//	cout<<"1================="<<endl;
				if(Edge_Matrix[target][*it]->GetLeft(target,*it) == 0)
				{
					it++;
					continue;
				}

				if(minn[target].bandWidth > Edge_Matrix[target][*it]->GetLeft(target, *it))
				{
					minn[*it].bandWidth = Edge_Matrix[target][*it]->GetLeft(target, *it);
				}

				else
					minn[*it].bandWidth = minn[target].bandWidth;


			//	cout<<"2================="<<endl;
				stack.push_back(*it);
				minn[*it].fee = 9999;
				minn[*it].fee = minn[target].fee + Edge_Matrix[target][*it]->fee;
				minn[*it].path.clear();
				minn[*it].path = minn[target].path;
				minn[*it].path.push_back(*it);

			}

			it++;
		//	cout<<"3=========================:"<<*it<<endl;
		}
	}

	//cout<<"4==================="<<endl;
	return minn;
}

//用spfa找到最佳候选服务器节点
vector<int> findCandidateNodesWithSPFA()
{
	
	for(int i = 0; i < consume_node_num; i++)
	{
		//找到每个消费节点到所有网络节点的费用最短路径
		ConsumeNode* consumeNode = &ALL_ConsumeNodes[i];
		BFSzy* bz = new BFSzy(consumeNode->net_node_id, consumeNode->bwNeed);
		spfa* minn = SPFAForCandidateServer(consumeNode->net_node_id, *bz);
        //找到每个消费节点到其他消费节点的路径，存放在ALL_Paths数组中
		for(int j = i + 1; j < consume_node_num; j++ )
		{
			vector<int>* temp = new vector<int>();
			ConsumeNode* cons = &ALL_ConsumeNodes[j];
			(*temp) =  minn[cons->net_node_id].path;
			//cout<<"down here "<<endl;
			ALL_Paths[consumeNode->net_node_id][cons->net_node_id] = temp;
			ALL_Paths[cons->net_node_id][consumeNode->net_node_id] = temp;
			//cout<<"down here ?"<<endl;
		}

		delete [] minn;
		delete bz;
	}

   //将路径上的节点根据出现的次数，将candiCount加加
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsumeNode* consumeNode = &ALL_ConsumeNodes[i];
		for(int j = i +1; j < consume_node_num; j++)
		{
			ConsumeNode* cons = &ALL_ConsumeNodes[j];
			vector<int>* temp = ALL_Paths[consumeNode->net_node_id][cons->net_node_id];
			for(int k = 1; k < temp->size() - 1; k++)//remove the first and the last
			{
				ALL_Nodes[(*temp)[k]].candiCount += 1;
			}
		}
	}
    //凡是出现次数大于等于2的标记为参考候选节点
	for(int i =0 ; i < node_num; i++)
	{
		if(ALL_Nodes[i].candiCount >= 2)
		{
			CandidateNodes.push_back(i);
		}
	}

#if 1
	cout<<"candidate node num: "<<CandidateNodes.size()<<endl;
	cout<<"candidate nodes: ";
	for(vector<int>::iterator it = CandidateNodes.begin(); it != CandidateNodes.end(); it++)
	{
		cout<<" "<<*it;
	}
	cout<<endl;
#endif

	return CandidateNodes;
}

