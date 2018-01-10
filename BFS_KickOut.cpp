#include "BFS_KickOut.h"

#include <deque>
#include "ALLSPACE_VARIABLES.h"
using namespace std;



/*
广度优先搜索+剪枝
*/
BFSzy BFSsearch(BFSzy& aant)
{
	int stopNum = 0;  
	int passed[10000];  //标记是否被访问过
	memset(passed,0,sizeof(passed));

	BFSzy temp_result;
	temp_result.cost = 65534;
	deque<bfs> stack;  //存放每一层的点

	for(int i = 0 ; i < aant.mulPath.size();i++)
	for(int j = 0 ; j < aant.mulPath[i].size();j++ )
		passed[aant.mulPath[i][j]] = 1;


    bfs* tmp =  new bfs();
    stack.push_back(*tmp);
    delete tmp;
    
	//队列中先放入当前节点
	stack[0].now_p = aant.currentPosition;
	stack[0].cost = aant.cost;
	stack[0].isStop = 0;
	stack[0].bandWidth = aant.bandWidth[0];
	stack[0].path.insert(stack[0].path.end(),aant.mulPath[0].begin(),aant.mulPath[0].end());


	while(true)
	{
		if(bfsNum == 100000)
		{
			//cout<< "temp_result0  :" <<temp_result.ToString()<< endl;
			return temp_result;
		}
		stopNum++;

		if(stopNum == 10000)
		{
			int is_Satisfy = 1;
			for(int i =0;i < stack.size();i++)
			{
				//cout<<"temp_cost: "<< temp_result.cost <<" , stack_cost"<<stack[i].cost<<endl;
				if(temp_result.cost > stack[i].cost)
				{
					is_Satisfy = 0;
					break;
				}
			}
			if(is_Satisfy == 1)
			{

				//cout<< "temp_result1  :" <<temp_result.ToString()<< endl;
				return temp_result;
			}
			stopNum = 0;
		}

		bfsNum++;
		string tmp;
		//cin>>tmp;
	//	cout<< " ================================================"<<endl;
		bfs temp_bfs;//get stack first element�ֵ//得到队列头部元素

		if(stack.size() > 0)
			temp_bfs = stack.front();//得到队列头部元素
		else
		{
			//cout << "temp_result2  :" << temp_result.ToString() << endl;
			return temp_result;
		}

		stack.pop_front();
		//cout<<" stack size : " << stack.size() << endl;
	//	cout<<"check currentNode:" << temp_bfs.now_p << " if crossedNode" << endl;
	//	cout << "path :";
	//将队列头部元素节点路径上的点全部标记为已经访问
		for(vector<int>::iterator _i = temp_bfs.path.begin();
			_i != temp_bfs.path.end(); _i++)
		{
			passed[*_i] = 1;
			//cout<< *_i << " , ";
		}
		//cout << endl;
        //type==2说明是交汇点
		if(ALL_Nodes[temp_bfs.now_p].type == 2 && temp_bfs.cost != 0)//������ڵ�λ���Ƿ��ߵ��˽����
		{
			//cout<<"reach crossed nodes" << endl;
			//将此交汇点的费用与之前的临时费用相比较
			if(temp_bfs.cost <= temp_result.cost)
			{
				//cout<<"better than currentSolution"<<endl;
				//temp_result置换
				temp_result.cost = 0;
				temp_result.cost = temp_bfs.cost;
				temp_result.mulPath.clear();
				temp_result.bandWidth.clear();
				temp_result.mulPath.push_back(temp_bfs.path);
				temp_result.bandWidth.push_back(temp_bfs.bandWidth);

				int is_Satisfy = 1;
				//将cost与队列中各点的费用相比较，看是否是最小，如果是最小，则迭代终止
				for(int i =0;i < stack.size();i++)
				{
	//				cout<<"temp_cost: "<< temp_result.cost <<" , stack_cost"<<stack[i].cost<<endl;
					if(temp_result.cost > stack[i].cost)
					{
						is_Satisfy = 0;
						break;
					}
				}
				if(is_Satisfy == 1)
				{
	//				cout<<"get temperory solution" << endl;
					//cout<< "temp_result3  :" <<temp_result.ToString()<< endl;
					return temp_result;
				}
			}
	//		cout<<"continue ??" <<endl;
			continue;
		}

        //当前节点的邻居节点
		list<int> temp_n = ALL_Nodes[temp_bfs.now_p].neighbors;//��ȡneighbors��ֵ
		vector<bfs_neighbor> nnn;
	//	cout<<"leave check" << endl;

	//	cout<< " find neighborInfo:" ;
	//遍历邻居节点，将邻居节点的信息放入nnn数组中
		list<int>::iterator it = temp_n.begin();
		while(it != temp_n.end())
		{
			//cout<< *it<<" ";
			//is passed by?
			if(passed[*it] == 1)
			{
				it++;
				continue;
			}
			bfs_neighbor temp_nnn;
			temp_nnn.id = *it;
			temp_nnn.fee = Edge_Matrix[temp_bfs.now_p][*it]->fee;
			temp_nnn.left = Edge_Matrix[temp_bfs.now_p][*it]->GetLeft(temp_bfs.now_p,*it);
			nnn.push_back(temp_nnn);
			it++;
		}
	//	cout<<endl;
		//buble sort by fee
		//对邻居节点进行费用排序，将费用最小的排到最前面
		for(int i = 0; i < nnn.size(); i++)
		for(int j = 0; j < nnn.size()-1;j++)
			if(nnn[j].fee > nnn[j+1].fee)
			{
				bfs_neighbor temp = nnn[j];
				nnn[j] = nnn[j+1];
				nnn[j+1] = temp;
			}
	//	cout << "leave find" << endl;
		/*
		 *
		 */
	//	cout << "decide which path to go" << endl;
		/*
			根据费用，从最小的开始选，如果第i条路的剩余带宽支持本次的全部流量，那么就直接打包塞到栈里面
			如果第i条路的剩余带宽不支持本次流量，那么就开始寻找从第i条路开始往后，找到第j条路完全满足流量之后，分成 j-i个蚂蚁，分别search，之后接着从j开始往后扫描
		*/
		for(int i=0;i < nnn.size() ; i++)
		{
		//	cout << "if bw satisfied nodeId" << nnn[i].id ;
		//流量满足的情况下
		     /*
				如果流量满足，打包塞入队列
			*/
			if(nnn[i].left >= temp_bfs.bandWidth && temp_bfs.bandWidth != 0)
			{

				//cout<<"id:"<<nnn[i].id<<endl;
				//cout<<"bandWidth:"<<temp_bfs.bandWidth<<endl;
			//	cout << "      satisfied"<<endl;
			//stackSizeControll控制广度优先搜索的宽度
				if(stack.size() > stackSizeControll) break;
				bfs temp;
				temp.now_p = nnn[i].id;
				temp.bandWidth = temp_bfs.bandWidth;
				temp.cost = temp_bfs.bandWidth * nnn[i].fee + temp_bfs.cost;
				//如果当前费用已经超过直连费用，则这个点不用考虑，直接考虑下一个节点
				if(temp.cost  > server_deploy_cost) continue;
				temp.path = temp_bfs.path;
				temp.path.push_back(nnn[i].id);
				stack.push_back(temp);//这个点放入队列中
				//cout << " leave decide " <<endl;
			}
			//如果流量不满足要求，或者当前蚂蚁的流量已经是0了
			/*
				如果分裂的话
			*/
			else
			{		
				//cout <<"     not satisfied" << endl;
				//如果当前蚂蚁流量已经是0，那么
				if(temp_bfs.bandWidth  == 0) break;
				//如果流量不满足要求，则从前往后搜索，直到流量满足要求
				int totalNeed = temp_bfs.bandWidth;
				BFSzy temp_div_r;
				temp_div_r.cost = temp_bfs.cost;
				int stop = 0;

				int j = i;
				int tte = 0;
				int first = 0;

				for(;;j++)
				{
					if(j > i && first == 0)
					{
						j = -1;
						first = 1;
						continue;
					}
					if(j == i && first == 1)
					{

						continue;
					}
					if( j == i && first == 0)
					{
						tte = tte + nnn[j].left;
					}


					if(j >= nnn.size()) break;
					if(nnn[j].left - temp_bfs.bandWidth > 0)
						tte += nnn[j].left - temp_bfs.bandWidth;
					else
						continue;
					if(tte > totalNeed) break;
				}

				if(tte < totalNeed) break;

				j = i;
				first = 0;
				for(;;j++)
				{
					if(j > i && first == 0)
					{
						j = -1;
						first = 1;
						continue;
					}
					if(j == i && first == 1)
					{

						continue;
					}

					if(stop || j >= nnn.size()) break;
					int bandWidth = 0;


					if(j == i)
					{
						if(totalNeed > nnn[j].left)
						{
							bandWidth = nnn[j].left;
							totalNeed = totalNeed - nnn[j].left;
						}
						else
						{
							bandWidth = totalNeed;
							stop = 1;
						}
					}
					else
					{
						/*
						if(totalNeed > nnn[j].left)
						{

								�����Է��ѣ���С�ķ��Ѵ��֮��search


							bandWidth = nnn[j].left - temp_bfs.bandWidth;
							if((bandWidth > (nnn[j].left - temp_bfs.bandWidth)))
								continue;
							totalNeed = totalNeed - nnn[j].left;
						}
						else
						{
							bandWidth = totalNeed;
							if((bandWidth > (nnn[j].left - temp_bfs.bandWidth))) continue;
							stop = 1;
						}
						*/
						if(nnn[j].left -temp_bfs.bandWidth < 0)
							continue;

						if(totalNeed > (nnn[j].left - temp_bfs.bandWidth))
						{
							bandWidth = nnn[j].left - temp_bfs.bandWidth;
							totalNeed = totalNeed - bandWidth;
						}

						else
						{
							bandWidth = totalNeed;
							stop = 1;
						}

					}


					BFSzy little,littler;
					little.currentPosition = nnn[j].id;
					little.cost = bandWidth * nnn[j].fee;
					little.bandWidth.push_back(bandWidth);
					little.mulPath.push_back(temp_bfs.path);
					little.mulPath[0].push_back(nnn[j].id);

					littler = BFSsearch(little);
					
					/*
						拿到分裂之后的返回值之后，要和现有的temp_bfs合并					
					*/

					temp_div_r.cost +=  littler.cost;
					for(int k = 0; k < littler.mulPath.size();k++)
					{
						temp_div_r.mulPath.push_back(littler.mulPath[k]);
						temp_div_r.bandWidth.push_back(littler.bandWidth[k]);
					}

				}
				if(i < nnn.size()) i = i + 1;
				/*if(j < nnn.size()) i = j - 1;*/
				/*
					�Ѵӷ����õ��ķ��������еķ������бȽϣ�ȷ���Ƿ�ʹ��
				*/
				//将分裂后最终得到的临时结果与之前的存储的费用相比，是否最低
				if(temp_div_r.cost < temp_result.cost)
				{
					temp_result = temp_div_r;
					//cout<<"temp_div_r:"<<endl;
					//cout<<temp_div_r.ToString()<<endl;
					/*
						�͵�ǰstack����ķ������бȽ�
					*/
					int isSatisfy = 1;
					for(int i=0;i < stack.size();i++)
					{
						if(temp_div_r.cost > stack[i].cost)
						{
							isSatisfy = 0;
							break;
						}
					}

					if(isSatisfy)
					{
						//cout<< "temp_result4  :" <<temp_result.ToString()<< endl;
						return temp_result;
					}
				}
			//	cout << "leave not satisfied" << endl;
			}
		}
	}
	return temp_result;
}




int calTotalJointCost(int *crossNodeIds,int joint_num)
{
	int ttr = 0;
	for(int i = 0; i < joint_num;i++)
	{
		ttr = ttr + ALL_Nodes[crossNodeIds[i]].antCosts;
	}
	return ttr;
}




//这段kickout的思想是：
/*
提出一个交汇点，首先看这个路径上是否还有其他交汇点，如果有其他交汇点，则移到其他交汇点，如果
没有其他交汇点，则重新BFS搜索
*/
int kickOut(int joint_num)
{

	int i =joint_num;

	int* preCrossNodeId = crossNodeIds;
	while(1)//if crossNodeId is all finished
	{
		//先把交汇点类型置为1
		ALL_Nodes[crossNodeIds[i--]].type = 1;

		int isAnthor;
		int antId;
		int isolateAntCost = 0;

		BFSzy refindr;
		vector<BFSzy> rv;
		vector<int> rvId;


		list<int>::iterator it = ALL_Nodes[crossNodeIds[i]].antsPassedBy.begin();
		for(;it != ALL_Nodes[crossNodeIds[i]].antsPassedBy.end(); it++)//search ants path ,see if it have anthor crossedNode
		{

			isAnthor = -1;//标记蚂蚁是否有其他交汇点
			antId = *it;

			Ant* it2 = find_Ant_In_All_Ants(antId);
            //看是否有其他交汇点
			if(it2->path.back() != crossNodeIds[i])
			{
				
				for(int j = 0;i < it2->path.size() ; j++)
				{
					if( ALL_Nodes[(*it2).path[j]].type == 2)
					{
						isAnthor = it2->path[j];
						break;
					}
				}
				//	如果有其他交汇点
				if(isAnthor != -1)//if ant has anthor JointPoint
				{
					int begin = 0;;
					ALL_Nodes[isAnthor].antsPassedBy.push_back(antId);//11111=======================================================================
                     //费用改变
					for(int j = 0; j < it2->path.size()-1 ; j++)
					{
						if(begin == 1)
						{
							it2->currentCost += Edge_Matrix[it2->path[j]][it2->path[j+10]]->fee * it2->bandWidth;
						}
						if( it2->path[j] == crossNodeIds[i])
							begin = 1;
						if( it2->path[j] == isAnthor)
							break;
					}

				}

			}
			//重新发现其他路径
			else//refind ant path
			{
				BFSzy refind(it2->path.front(),it2->bandWidth);
				refindr = BFSsearch(refind);
				isolateAntCost += refindr.cost;
				rv.push_back(refindr);
				rvId.push_back(antId);

			}
		}
//将kickout之后的费用与之前的费用相比较，看是否变低了
		if(calTotalJointCost(preCrossNodeId,i+1) > ( calTotalJointCost(crossNodeIds,i) + isolateAntCost))
		{
			return 0;
		}
		else
		{
			for(int j = 0; j < rv.size();j++)
			{
				for(int k = 0 ; k < rv[j].mulPath.size();k++)
				{
					int temp_c = 0;

					for(int l = 0 ; l < rv[j].mulPath[k].size()-1;l++)
					{
						int nn = rv[j].mulPath[k][l];
						int mm = rv[j].mulPath[k][l+1];
						//Edge_Matrix[nn][mm].RequireForBW(nn ,mm ,rv[j].bandWidth[k]);
						temp_c = temp_c + rv[j].bandWidth[k] * Edge_Matrix[rv[j].mulPath[k][l]][rv[j].mulPath[k][l+1]]->fee;
					}
					Ant antt;
//					antt.id = ants.back()->id + 1;
//					antt.currentCost = temp_c;
//					antt.bandWidth = rv[j].bandWidth[k];
//					antt.path = rv[j].mulPath[k];
//					ants.push_back(&antt);
//					ALL_Nodes[rv[j].mulPath[k][rv[j].mulPath[k].size()]].antCosts += temp_c;
//					ALL_Nodes[rv[j].mulPath[k][rv[j].mulPath[k].size()]].antsPassedBy.push_back(antt.id);
				}
			}
		}

	}

	return 0;
}





















