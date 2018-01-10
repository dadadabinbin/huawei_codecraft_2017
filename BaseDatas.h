/*
 * BaseDatas.h
 *
 *  Created on: Mar 15, 2017
 *      Author: root
 */
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "ALLSPACE_VARIABLES.h"


#ifndef BASEDATAS_H_
#define BASEDATAS_H_

using namespace std;

list<int> FindConsNodesWithoutServer();

list<int>& remove(list<int>& _list, int value)
{
	list<int>::iterator i = _list.begin();
	for(; i != _list.end(); )
	{
		if(*i == value)
		{
			i = _list.erase(i);
		}
		else{
			i++;
		}
	}
	return _list;
}

vector<string> split(string& str,const char* c)
{
    char *cstr, *p;
    vector<string> res;
    cstr = new char[str.size()+1];
    strcpy(cstr,str.c_str());
    p = strtok(cstr,c);
    while(p!=NULL)
    {
        res.push_back(p);
        p = strtok(NULL,c);
    }
    delete cstr;
    return res;
}


void GenerateAllBandWidthMemory()
{
	for(int i=0;i<edge_num;i++)
	{
		ALL_Edges[i].GenerateMemory();
	}
}

void RecoreAllNamdWidthMemory()
{
	for(int i=0;i<edge_num;i++)
	{
		ALL_Edges[i].RestoreToLastMemory();
	}
}

void Initialization(char* filecontent[], int numOflines)
{
	//将第一个参数变成string
	string lineContent(filecontent[0]);
	//文本用空格分隔放入容器
	vector<string> contentStrs = split(lineContent, " ");

	node_num =atoi(contentStrs[0].c_str());
	edge_num = atoi(contentStrs[1].c_str());
	consume_node_num = atoi(contentStrs[2].c_str());
	//在消费节点数目不一致的时候选择不同的BFS的搜索宽度，也就是栈的大小
	//stackSizeControll控制BFS里面队列的宽度，也就是控制BFS搜索的宽度，当消费节点越多的时候，我们剪枝的越多
	if(consume_node_num < 100) stackSizeControll = 1000;
	else
		if(consume_node_num < 400)
			stackSizeControll = 10;
		else
			stackSizeControll = 2;
	//初始情况每一个消费节点放置一个蚂蚁，所以蚂蚁数目等于消费节点数目
	ants_num = consume_node_num;

	cout<<"node_num: " << node_num<<endl;
	cout<<"edge_num: " << edge_num<<endl;
	cout<<"consume_node_num: " << consume_node_num << endl;

	//分配空间
	ALL_Nodes = new Node[node_num];
	ALL_Edges = new Edge[edge_num];
	Edge_Matrix = new Edge**[node_num];
	ALL_ConsumeNodes = new ConsumeNode[consume_node_num];
	
	//初始交汇点数组分配消费节点数目那么大空间，也就是默认交汇点数目不超过消费节点数目，后续如果要寻找更多交汇点
	//这个空间可以扩大容量
	crossNodeIds = new int[consume_node_num];

	for(int i = 0 ; i < node_num; i++)
	{
		Edge_Matrix[i] = new Edge*[node_num];
	}
	//第三行是服务器的费用
	server_deploy_cost = atoi(filecontent[2]);
	cout<<"initial arrays finished , server_deploy_cost "<<server_deploy_cost << endl;

	//initial ALL_Nodes
	for(int i = 0; i < node_num; i++){
		Node* tempNode = new Node();
		ALL_Nodes[i] = *(tempNode);
		delete tempNode;
	}
	cout<<"initial ALL_Nodes finished"<<endl;
	//initial ALL_edges
	//从第四行开始初始化每条边
	for(int i = 0, j = 4; i < edge_num; i++, j++)
	{
		string* tempStr = new string(filecontent[j]);
		lineContent = *(tempStr);
		delete tempStr;

		contentStrs = split(lineContent, " ");
		if(contentStrs.size() != 4)
		{
			cout<<"!!!!ERROR initial Edges i: " << i << ", j = " << j<<endl;
			return;
		}
        //c_str先转化为const char*，再atoi转化为int
		int start = atoi(contentStrs[0].c_str());
		int end = atoi(contentStrs[1].c_str());
		int bandWidth = atoi(contentStrs[2].c_str());
		int fee = atoi(contentStrs[3].c_str());

		Edge* tempEdge = new Edge(start, end, bandWidth, fee);
		ALL_Edges[i] = *(tempEdge);
		delete tempEdge;

		//set neighbor
		ALL_Nodes[start].AddNeighbor(end);
		ALL_Nodes[end].AddNeighbor(start);

		//set matrix
		//邻接矩阵放的是地址
		Edge_Matrix[start][end] = &ALL_Edges[i];
		Edge_Matrix[end][start] = &ALL_Edges[i];
	}
	cout<<"initial edges finished" << endl;

	//initial consumeNodes and Ants
	//j表示文件中对应的行数
	for(int i = 0, j = 5 + edge_num; i < consume_node_num; i++,j++)
	{
		//consumeNodes
		string* tempStr = new string(filecontent[j]);
		lineContent = *(tempStr);
		delete tempStr;
		
		contentStrs = split(lineContent, " ");
		//消费节点id就是消费节点存放数组的下标，所以这个弃之不用了
//		int consumeNodeid = atoi(contentStrs[0].c_str());
		int nodeId = atoi(contentStrs[1].c_str());
		int bandWidth = atoi(contentStrs[2].c_str());
		ALL_Nodes[nodeId].type = 1;   //type为1表示他是消费节点所在的是普通网络节点，type为2表示是交汇点
		ALL_Nodes[nodeId].bwNeed = bandWidth;

		ConsumeNode* cnTmp = new ConsumeNode(nodeId, bandWidth);
		ALL_ConsumeNodes[i] = *cnTmp;
		delete cnTmp;
		//Ants
		Ant* ant = new Ant(nodeId, bandWidth);
		ants[ant->id] = ant;
	}

	ALL_Paths = new vector<int>**[node_num];
	//initial All Paths
	for(int i = 0 ; i < node_num; i++)
	{
		ALL_Paths[i] = new vector<int>*[node_num];
	}
	//用spfa找到候选参考服务器点
	findCandidateNodesWithSPFA();
	cout<<"initial Candidate nodes finished"<<endl;
	cout<<"initial finished!"<<endl;


}

bool Contains(list<int> _list, int value)
{
	for(list<int>::iterator i = _list.begin(); i != _list.end(); i++)
	{
		if(*i == value)
		{
			return true;
		}
	}
	return false;
}


void PrintAnts()
{
	cout << endl << "Ants:" << endl;
	for(map<int,Ant*>::iterator it = ants.begin();
			it != ants.end(); it++)
	{
		cout<<(*it).second->ToString() << endl;
	}
}

void PrintJointNodes()
{
	cout<<endl<<"Joints_Num: " <<joint_num<<endl;
	for(int i = 0; i < joint_num; i++)
	{
				int nodeId = crossNodeIds[i];
				cout<<"==========================================================="<<endl;
				cout<<"joint_nodeId :" << nodeId<<endl;
				if(nodeId >= node_num)
				{
					cout <<"node_Id" << nodeId << endl;
					cout <<"joint_num" << joint_num<<endl;
					int _i = consume_node_num;
					cout <<"consume_node_num " << _i <<endl;
					break;
				}
				cout<<"num_of_antsPassedby: " << ALL_Nodes[nodeId].antsPassedBy.size()<<endl;
				for(list<int>::iterator it = ALL_Nodes[nodeId].antsPassedBy.begin();
						it != ALL_Nodes[nodeId].antsPassedBy.end(); it++)
				{
					Ant* ant = find_Ant_In_All_Ants(*it);
					cout<< ant->ToString();
				}
				cout << endl;
	}
}

void PrintConsumeNodes()
{
  cout<<endl<<"ConsumeNodes: ";
  for(int i = 0; i < consume_node_num;i++)
  {
	  cout<<ALL_ConsumeNodes[i].net_node_id<<" , ";
  }
  cout<<endl;
}

void PrintAllEdges()
{
	for(int i = 0; i < edge_num; i++)
	{
		cout<<ALL_Edges[i].ToString();
		cout<<endl;
	}
}
bool LevelCompare(int a, int b)
{
	return ALL_Nodes[a].kickLevel > ALL_Nodes[b].kickLevel;
}

void SortListByLevel(list<int>* ids)
{
	ids->sort(LevelCompare);
}

bool IdCompare(int a, int b)
{
	return a < b;
}
void SortListById(list<int>* ids)
{
	ids->sort(IdCompare);
}
void PrintAllNodesLevel()
{
	cout<<"All Nodes level : ";
	list<int> allNodeIds;
	for(int i = 0 ; i < node_num; i++)
	{
		allNodeIds.push_back(i);
		//cout << " " << i << " : " << ALL_Nodes[i].kickLevel<< endl;
	}

	SortListByLevel(&allNodeIds);

	for(list<int>::iterator it = allNodeIds.begin(); it != allNodeIds.end(); it++)
	{
		cout << " " << *it << " : " << ALL_Nodes[*it].kickLevel<< endl;
	}
	cout<<endl;
}

void PrintAllServerLevel()
{
	cout<<"All Server level : ";
	SortListById(&solution->serverIds);
	for(list<int>::iterator i = solution->serverIds.begin(); i != solution->serverIds.end(); i++)
	{
		cout<< " " << *i << " : " << ALL_Nodes[*i].kickLevel<<endl;
	}
	cout<<endl;
}



/*
	功能：得到交汇点List
	函数名：GetJointClass
	参数：
*/
list<JointPoint*> GetJointPoints()
{
	int count = 0;
	for(int i = 0 ; i < node_num; i++)
	{
		//当此节点的不同消费节点来的蚂蚁数目大于等于2
			if(ALL_Nodes[i].RealAntsCount().size() >= 2)
			{
				crossNodeIds[count++] = i;
				ALL_Nodes[i].type = 2;
				ALL_Nodes[i].CalculateAntCosts();
				//当交汇点数目等于消费点数目的时候，将crossNodeIds数组扩大三倍
				if(count == consume_node_num)
				{
					cout<<"crossNodes extended !!!!"<<endl;
					int* tmp = crossNodeIds;
					crossNodeIds =  new int[consume_node_num * 3];
					memcpy(crossNodeIds, tmp, sizeof(int) * consume_node_num);
					delete []tmp;
				}
			}
	}
    //joint_num记录交汇点的数目
	joint_num = count;
	return ALL_JointPoints;//这个返回值之后没有用到
}

/*
	功能：检查是否满足停止条件，即所有的蚂蚁都已经停止，找到了可能最佳部署服务器的位置
		  停止条件：
			1.交汇点数目达到消费节点的两倍
			2.当单个蚂蚁的路径消耗超过部署单台服务器的费用时，停止移动。当所有蚂蚁停止移动。
	函数名：CheckIfALLStoped
	参数：
	返回值：bool
*/
bool CheckIfAllStoped()
{
	/*
		先检查交汇点数目，在检查蚂蚁停止情况
	*/
	if(joint_num >= consume_node_num*2 )
	{
		cout<<"max JointPoints"<<endl;
		return true;
	}

	int sumOfStop=0;
    //检查是否所有蚂蚁都停止了
	for(map<int,Ant*>::iterator it=ants.begin();it!=ants.end();it++)
	{
		if((*it).second->isStoped)
			sumOfStop++;
	}
	if(sumOfStop == ants.size())
	{
		cout<<"all ants stopped , ants num : " << ants.size()<<endl;
		return true;
	}
	return false;

}

//用来判断蚂蚁下一步的结果
NextDestination NextStep(Ant* ant)
{
	    //cout<<"enter NextStep" << endl;
		NextDestination nd;//建立返回结构

		if(ant->isStoped == true)
		{
			nd.length = -1;
			return nd;
		}

		if(ant->currentCost > server_deploy_cost)
		{
			nd.length = -2;
			return nd;
		}
	//	cout<<"ant cost check "<<", antId:" <<ant->id << " ,cost : " << ant->currentCost << endl;
        //下一步的节点不包括曾经考虑的节点
		list<int> possibleNextStep;
		possibleNextStep = ALL_Nodes[ant->currentPosition].neighbors;

	//	int possibleNextStepSize=ALL_Nodes[ant->currentPosition].neighbors.size();//可能节点的大小

			for(list<int>::iterator itabNode = ant->abandonedNodes.begin();
					itabNode!=ant->abandonedNodes.end();
					itabNode++)
			{
				 for(list<int>::iterator itNextStep = possibleNextStep.begin();
						 itNextStep!=possibleNextStep.end();)
				 {
				 		   if(*itNextStep==*itabNode)
				 			{
				 			  itNextStep = possibleNextStep.erase(itNextStep);;
				 			}
				 		   else
				 		   {
				 			   itNextStep++;
				 		   }
				 }
			}
		/*
			将费用最小的节点进行排序
		*/
		   //在去除完不可达节点的情况下，寻找最小花费的节点
	//		cout<<"core dump 1?"<<endl;
			int* pns=new int[possibleNextStep.size()];
			int pns_l = 0;
			for(list<int>::iterator it = possibleNextStep.begin();it!=possibleNextStep.end();it++)
			{
				pns[pns_l++] = *it;
			}
	//		cout<<"not here"<<endl;

			//做边费用的排序
	//		cout<<"core dump 2?"<<endl;
		   for(int i=0;i<pns_l;i++)
		   {
				for(int j=0;j<pns_l-1;j++)
				{
					if(Edge_Matrix[ant->currentPosition][pns[j]]->fee > Edge_Matrix[ant->currentPosition][pns[j+1]]->fee)
					{
						int temp = pns[j];
						pns[j] = pns[j+1];
						pns[j+1] = temp;
					}
				}
		   }
	//		cout<<"not here"<<endl;
		/*
			走下一步，建立NextDestination的结构
		*/
		 //判断从最小边开始的带宽是否符合带宽要求
		 //这里判断了流量不满足的情况
		   int bwNeed = ant->bandWidth;
		   nd.length=0;
		   for(int i = 0 ; bwNeed > 0 && i < pns_l ;  i++)
		   {
			   if(ant->currentPosition == pns[i])
			   {
				   cout<<"Exception in nextStep !!!! " << " ant->currentPosition equal pns[i]"<<endl;
			   }
			   int left = Edge_Matrix[ant->currentPosition][pns[i]]->GetLeft(ant->currentPosition,pns[i]);
               
			   if(left <= 0)//no more bandWidth to offer
				   	  continue;

			   nd.nextNodeIds.push_back(pns[i]);
			   if(bwNeed >= left)
			   {
			//	   Edge_Matrix[ant->currentPosition][pns[i]].RequireForBW(ant->currentPosition,pns[i], left);
				   bwNeed -= left;
				   nd.bandWidths.push_back(left);
			   }
			   else{
			//	   Edge_Matrix[ant->currentPosition][pns[i]].RequireForBW(ant->currentPosition,pns[i], left - bwNeed);
				   nd.bandWidths.push_back(bwNeed);
				   bwNeed = 0;
			   }
			   nd.length++;
		   }
		   if(bwNeed > 0)
		   {
			   cout<<"no way to go, stopped ant Id :" << ant->id <<endl;
			   nd.length = -3;
		   }
		   nd.lastNodeId=ant->currentPosition;
	//	   cout << "leave next Step" << endl;
		   return nd;
}

/*
*
*  根据每次的下一步结果链表判断
*  维护ants队列，出现分裂情况，删除当前ant，并添加新的分裂ant
*/
void MaintainAnts(NextDestination ndt,Ant* ant)
{
	//分裂的情况
		if(ndt.length>=2)//divided
		{
				cout<<"get in 2"<<endl;
				list<int>::iterator nodeIter = (ndt).nextNodeIds.begin();
				list<int>::iterator bwIter = (ndt).bandWidths.begin();
				//ndt.length是增加蚂蚁的个数
				for(int i=0;i< ndt.length;i++, nodeIter++, bwIter++)
					{
						//增加的蚂蚁
						//注意这里蚂蚁id是自动增加的
						Ant* addAnt = new Ant();
						//cout<<"nextNodeId : " <<*nodeIter << endl;
						addAnt->currentPosition = *nodeIter;
						//更新边的流量
						Edge_Matrix[ndt.lastNodeId][addAnt->currentPosition]->RequireForBW(ndt.lastNodeId,addAnt->currentPosition,*bwIter);
						addAnt->bandWidth = *bwIter;
						addAnt->path = (*ant).path;
						addAnt->path.push_back(addAnt->currentPosition);
						addAnt->abandonedNodes = (*ant).abandonedNodes;
						addAnt->abandonedNodes.push_back(addAnt->currentPosition);
						addAnt->isStoped = (*ant).isStoped;
						addAnt->currentCost = addAnt->CalculateCost();
						addAnt->stoppedReason = (*ant).stoppedReason;
						
						//对蚂蚁经过的节点上记录经过的蚂蚁
						for(int j = 0; j < addAnt->path.size(); j ++)
						{
							ALL_Nodes[addAnt->path[j]].OnAntPassedBy(addAnt->id);
						}
						//增加的蚂蚁放入蚂蚁数组，用数组方式插入map类型数据
						ants[addAnt->id] = addAnt;
					}

		}else if(ndt.length == 1){//没有分裂的情况
			cout<<"get in 1"<<endl;
		    //增加考虑的节点
			for(list<int>::iterator itabNode=ALL_Nodes[ant->currentPosition].neighbors.begin();itabNode!=ALL_Nodes[ant->currentPosition].neighbors.end();itabNode++)
				(*ant).abandonedNodes.push_back((*itabNode));
			(*ant).currentPosition=*(ndt.nextNodeIds.begin());
			(*ant).path.push_back((*ant).currentPosition);
			(*ant).currentCost = (*ant).CalculateCost();
			Edge_Matrix[ndt.lastNodeId][ant->currentPosition]->RequireForBW(ndt.lastNodeId,ant->currentPosition,ant->bandWidth);
			ALL_Nodes[ant->currentPosition].OnAntPassedBy(ant->id);
		}

		else if(ndt.length == -1)//
		{
			cout<<"get in -1"<<endl;
		    (*ant).isStoped = true;
		}
		else if(ndt.length == -2)
		{
			cout<<"get in -2" << endl;
			(*ant).isStoped = true;
			(*ant).stoppedReason = -2;
		}
		else if(ndt.length == -3)
		{
			cout<<"get in -3" << endl;
			(*ant).isStoped = true;
			(*ant).stoppedReason = -3;
		}
}
//根据给定交汇点，生成最终的方案solution,
Solution*  GenerateSolutionBasedOnServers(list<int> Ids)
{
	//at the place of cross ,set server
	solution = new Solution();
	 //calculate server cost
	solution->cost=0;

	//get index of crossNode
	vector<Ant*> temp_Ant;//get all of ants
	for(list<int>::iterator itCross = Ids.begin();itCross != Ids.end();itCross++)
	{
		for(list<int>::iterator itAnt=ALL_Nodes[*itCross].antsPassedBy.begin();itAnt!=ALL_Nodes[*itCross].antsPassedBy.end();itAnt++)
		{
			vector<Ant*>::iterator itSameAnt;
			for(itSameAnt=temp_Ant.begin();itSameAnt!=temp_Ant.end();itSameAnt++)
			{
				if((*itSameAnt)->id==find_Ant_In_All_Ants(*itAnt)->id)
				{
					//(*itSameAnt)->bandWidth+=find_Ant_In_All_Ants(*itAnt)->bandWidth;
					break;
				}

			}
			if(itSameAnt==temp_Ant.end())
			     temp_Ant.push_back(find_Ant_In_All_Ants(*itAnt));
		}
	}

//	for(vector<Ant*> ::iterator it=temp_Ant.begin();it!=temp_Ant.end();it++)
//	{
//		cout<<"ant:"<<(*it)->id<<endl;
//	}
	//give space to bwSolution
	solution->bwSolution=new ConsNode_bw_solution[consume_node_num];
	for(int i=0;i<consume_node_num;i++)
	{
		solution->bwSolution[i].cons_node_id=i;
		solution->bwSolution[i].isSatisfied=false;
	}

	//get firse net node id all the ants
	//get all the bwSolution path num
	for(vector<Ant*>::iterator itCN=temp_Ant.begin();itCN!=temp_Ant.end();itCN++)
	{
		for(int i=0;i<consume_node_num;i++)
		{
			if(ALL_ConsumeNodes[i].net_node_id==(*itCN)->path[0])
				solution->bwSolution[i].path_num++;
		}

	}

	//give the space of bwSolution[i] paths and bws
	for(int i=0;i<consume_node_num;i++)
	{
		if(solution->bwSolution[i].path_num>0)
		{
			solution->bwSolution[i].paths = new list<int>[solution->bwSolution[i].path_num];
			solution->bwSolution[i].bws = new int[solution->bwSolution[i].path_num];
			for(int j=0;j<solution->bwSolution[i].path_num;j++)
			{
				solution->bwSolution[i].bws[j]=-1;//init value is -1,to judge
			}
		}

	}

	for(vector<Ant*>::iterator itAnt=temp_Ant.begin();itAnt!=temp_Ant.end();itAnt++)
	{
		//every ant
		int i=0;
		//find is what bwsolution
		for(;i<consume_node_num;i++)
		{
			if(solution->bwSolution[i].path_num>0&&(ALL_ConsumeNodes[i].net_node_id==(*itAnt)->path[0]))
			{
					break;
			}
		}
    	list<int>  tempPath;
    	tempPath.clear();
    	int flag=true;//to jump two loop
        for(vector<int>::iterator itNode=(*itAnt)->path.begin();itNode!=(*itAnt)->path.end()&&flag==true;itNode++)
        {
        	//cout<<"path[i]"<<*itNode<<endl;
        	tempPath.push_back((*itNode));
        	for(list<int>::iterator itCross= Ids.begin();itCross!= Ids.end()&&flag==true;itCross++)
        	{
        		if(*itNode==*itCross)
        		{
        			list<int>::iterator itId=solution->serverIds.begin();
        			for(;itId!=solution->serverIds.end();itId++)
        			{
        				if(*itNode==(*itId))
        				{
        					break;
        				}
        			}
        			if(itId==solution->serverIds.end())
        			{
        				solution->serverIds.push_back(*itNode);//get serverId of solution
        			}
        			flag=false;
        		}
        	}
        }
    //    cout<<"this is path temp"<<endl;
//		for(list<int>::iterator itId=tempPath.begin();itId!=tempPath.end();itId++)
//		{
//		   cout<<*itId<<endl;
//		}
//		cout<<"-------------------------------------------------------"<<endl;

            for(int j=0;j<(solution->bwSolution[i].path_num);j++)
            {
            	if(solution->bwSolution[i].bws[j]==-1)
            	{
                    solution->bwSolution[i].paths[j]=tempPath;
                    solution->bwSolution[i].bws[j]=(*itAnt)->bandWidth;
                    break;
            	}
            }
	}
	for(int i=0;i<consume_node_num;i++)
	{
		if(solution->bwSolution[i].path_num>0)
		{
			solution->bwSolution[i].cost=solution->bwSolution[i].CostCalculate();
		}
	}

   //get the sum of every consume_node_num
	for(int i=0;i<consume_node_num;i++)
	{
		if(solution->bwSolution[i].path_num>0)
		{
		  solution->cost+=solution->bwSolution[i].cost;
		}
	}

	for(int i=0;i<consume_node_num;i++)
	{
		if(solution->bwSolution[i].path_num>0)
		{
		  int sumBandWidth=0;
		  for(int j=0;j<solution->bwSolution[i].path_num;j++)
			sumBandWidth+=solution->bwSolution[i].bws[j];
		  if(ALL_ConsumeNodes[i].bwNeed == sumBandWidth)
			  solution->bwSolution[i].isSatisfied=true;
		  else
			  solution->bwSolution[i].isSatisfied=false;
		}
	}
	solution->CalculateCost();

	return solution;
}


//将结果输出到指定文件中去
string OutputResult()
{
	   // ostringstream outfile("output: ");
	   string outfile;

		cout<<"...........this is output..................."<<endl;
		cout<<"cost: "<<solution->cost<<endl;

        int sumPath=0;
    	for(int i=0;i<consume_node_num;i++)
    	{
    		if(solution->bwSolution[i].path_num>0)
    		{
    			sumPath+=solution->bwSolution[i].path_num;
    		}
    	}
		cout <<"(*solution).path_num="<<sumPath;

	    char a[20];
	    sprintf(a, "%d\n\n",sumPath);
	    outfile=a;

		cout<<endl;

		stack<int> temp_stack;
		for(int i = 0; i < consume_node_num; i++)
		{
			for(int j = 0; j < (*solution).bwSolution[i].path_num;j++)
			{
				//output a path
				for(list<int>::iterator it= (*solution).bwSolution[i].paths[j].begin();it!= (*solution).bwSolution[i].paths[j].end();it++)
				{
					temp_stack.push(*it);

				}
			    while(!temp_stack.empty()){
			    	//outfile<<temp_stack.top()<<"  ";
			    	sprintf(a, "%d ",temp_stack.top());
			    	outfile+=a;
			    	//outfile+=" ";
			        cout<<temp_stack.top()<<"  ";
			        temp_stack.pop();

			    }
			   // outfile+="\b";
			    sprintf(a, "%d ",(*solution).bwSolution[i].cons_node_id);
			    outfile+=a;
				//outfile<<(*solution).bwSolution[i].cons_node_id<<"  ";
				cout<<"     cons_node_id="<<(*solution).bwSolution[i].cons_node_id<<"  ";
				sprintf(a, "%d",(*solution).bwSolution[i].bws[j]);
				outfile+=a;
				//outfile<<(*solution).bwSolution[i].bws[j];
				if(!(i == consume_node_num -1 && j == (*solution).bwSolution[i].path_num - 1))
				         outfile+="\n";
				cout<<" bws  "<<(*solution).bwSolution[i].bws[j];
				cout<<endl;


			}
		}
		return outfile;
}
void ReInitAllBandWidths()
{
	for(int i = 0; i < edge_num; i++)
	{
		ALL_Edges[i].ReInitBandWidth();
	}
	cout<<"ReInitial all badnWidths" << endl;
	//PrintAllEdges();
}

//蚂蚁运动
void AntsRunning()
{
	//ttt控制蚂蚁前进次数，不能超过1000,防止进入死循环
	int ttt = 0;
	while(!CheckIfAllStoped() && ttt < 1000)
	{
		ttt++;
		//蚂蚁数组的大小
		int sizeOfant = ants.size();
		int i = 0;
		//PrintAnts();
		//用图这个数据结构，图的第二位表示蚂蚁，第一位表示？  问题
		for(map<int,Ant*>::iterator it = ants.begin(); i < sizeOfant; i++)
		{
			//看蚂蚁是否停止
			Ant* ant = (*it).second;
			if(ant->isStoped)
			{
				it++;
				continue;
			}
            //构造返回结构
			NextDestination ndt = NextStep(ant);
			MaintainAnts(ndt, ant);

			if(ndt.length < 2)
			{
				it++;
			}
			else{
				//删除分裂前的蚂蚁
				Ant* ant = (*it).second;
				ants.erase(it++);
				delete ant;
			}
//			string str;
//			cin >> str;
		}
		GetJointPoints();
	}
	//由于边的带宽设置成动态变化的了，所以要恢复成一开始的带宽
	ReInitAllBandWidths();
	cout<<"ttt: "<<ttt<<endl;
	//PrintAllEdges();
}

//消费节点直连方案
list<int> FindConsNodesWithoutServer()
{
	list<int> result;
	for(int k = 0; k < consume_node_num; k++)
	{
		int consNodeId = ALL_ConsumeNodes[k].net_node_id;
		result.push_back(consNodeId);
		for(int i = 0; i < joint_num; i++)
		{
			int nodeId = crossNodeIds[i];
			for(list<int>::iterator it = ALL_Nodes[nodeId].antsPassedBy.begin();
					it != ALL_Nodes[nodeId].antsPassedBy.end();it++)
			{
				if(find_Ant_In_All_Ants(*it)->path[0]== consNodeId)
				{
						remove(result, consNodeId);
				}
			}
		}
	}

	cout<<"consNodesWithOutServer: ";
	for(list<int>::iterator it =result.begin(); it != result.end(); it++)
	{
		cout<<*it << " , ";
	}
	cout<<endl;
	return result;
}

//初始化服务器方案
bool InitialServerSolution()
{
	//find candidate servers
	list<int> ids;
	for(int i = 0 ; i < joint_num; i++)
	{
		ids.push_back(i);
	}
	cout<<"enter generateSolutionBasedOnServers"<<endl;
	//根据给定交汇点，生成最终的方案solution
	Solution* _solution = GenerateSolutionBasedOnServers(ids);
	cout<<"leave generateSolutionBasedOnServers"<<endl;
	//此时的serverids只包含供应两个及以上的消费节点的服务器节点
	_solution->GetBestCandidateServers();

	//find candidate servers end;
	ReInitAllBandWidths();

	Solution* resultSolution = new Solution();


	resultSolution->serverIds = _solution->serverIds;
	resultSolution->bwSolution = new ConsNode_bw_solution[consume_node_num];
	int lastCost = 999999;//last solution Cost , for kick out
	solution->cost = 999999;//for big round
	while(!resultSolution->IsAllNodeSatisfied())
	{
		
		bool StartKickOut = false; //for kick out  //标记是否踢服务器
		int kickOutTime = 0;//for kick out         //标记踢服务器的次数，在这里凡是直连的服务器都要踢一下
		while(!resultSolution->IsAllNodeSatisfied())
		{
			while(!resultSolution->IsAllNodeSatisfied())
			{
				for(int i = 0; i < consume_node_num; i++)
				{
					//cons存放最终的解决方案
					ConsNode_bw_solution* cons = resultSolution->bwSolution + i ;
					ConsNode_bw_solution* cons2 = _solution->bwSolution + i ;
					cons->cons_node_id = i;

					if(cons2->isSatisfied)//use an already generated consnode_bw_solution
					{
						if(cons2->path_num == 0)
						{
							cout<<"Exception how this come out??"<<endl;
							cout<<cons2->ToString()<<endl;
							//string str;
							//cin>>str;
						}
						cons->path_num = cons2->path_num;
						cons->bws = new int[cons2->path_num];
						cons->paths = new list<int>[cons2->path_num];
						for(int __i = 0 ; __i < cons2->path_num; __i++)
						{
							cons->bws[__i] = cons2->bws[__i];
							cons->paths[__i] = cons2->paths[__i];
						}
						cons->cost = cons->CostCalculate();
						//标记cons初始化成功
						cons->Initialized  = true;
						//如果此消费节点总花费小于直连服务器费用
						if(cons->cost  < server_deploy_cost)
						{
							if(cons->TakeBandWidths())//检查流量是否满足要求
							{
								cons->isSatisfied = true;//BFSSavedTimes的作用？
								BFSSavedTimes += 1;//how many time bfs search was saved，说明对每个满足要求的消费节点都要进行spfa搜索
							}
							else
							{
								cons->isSatisfied = false;
								cons->path_num = 0;
							}
						}
						else
						{
							cons->isSatisfied = false;
						}
						cons2->isSatisfied = false;//in case repeat set
					}

				if(!cons->isSatisfied)//当不满足的时候
				{
					if(cons->Initialized)//当已经初始化了，这时候就删除掉已经初始化的元素
					{
						delete [] cons->paths;
						delete [] cons->bws;
					}
					cout << cons->cons_node_id << " Not Satisfied!!!"<<endl;
					ConsumeNode* cn = &ALL_ConsumeNodes[i];
                    //所有边的流量全部恢复成初始状态
					GenerateAllBandWidthMemory();
					//对这个孤立节点进行spfa搜索
					BFSzy* bz = new BFSzy(cn->net_node_id, cn->bwNeed);
					bfsNum = 0;
					//(*bz) = BFSsearch(*bz);
					//string sss;
					//cin>>sss;
					*bz = SPFA(cn->net_node_id, cn->bwNeed, resultSolution->serverIds, (*bz));
					cout<<bz->ToString()<<endl;
					//所有边的流量全部恢复成刚才的状态
					RecoreAllNamdWidthMemory();
					//将spfa所得到的结果放入消费节点方案中
					if(bz->cost != 65534 && bz->cost != 0 && bz->cost < server_deploy_cost)
					{
						cout<<"consNodeId "<< i <<" find plan! " <<endl;
						int list_length = bz->mulPath.size();
						cons->paths = new list<int>[list_length];
						cons->bws = new int[bz->bandWidth.size()];
						cons->path_num = list_length;
						cons->isSatisfied = true;
						cons->Initialized = true;

						for(int _i = 0; _i < bz->mulPath.size(); _i++)
						{
							cons->bws[_i] = bz->bandWidth[_i];
							for(int j = 0 ; j < bz->mulPath[_i].size(); j++)
							{
								cons->paths[_i].push_back(bz->mulPath[_i][j]);
							}
						}
                         //检查流量是否满足，一般情况下流量都满足
						if(!cons->TakeBandWidths())
						{
							cons->isSatisfied = false;
						}
					}
                    //如果流量还是不满足，直接直连，保护方案
					if(!cons->isSatisfied)
					{
						cout<<"consNodeId "<< i <<" deploy server in " << cn->net_node_id << endl;
						//create new server
						if(!Contains(resultSolution->serverIds, cn->net_node_id))
						{
							crossNodeIds[joint_num++] = cn->net_node_id;
							ALL_Nodes[cn->net_node_id].type = 2;
							resultSolution->serverIds.push_back(cn->net_node_id);
						}

						cons->path_num = 1;
						cons->paths = new list<int>[1];
						cons->paths[0].push_back(cn->net_node_id);
						cons->bws = new int[1];
						cons->bws[0] = ALL_ConsumeNodes[cons->cons_node_id].bwNeed;
						cons->cost = 0;
						cons->isSatisfied = true;
						cons->Initialized = true;

						if(!StartKickOut)//直连的服务器一般都要剔除一下试试
						{
							kickOutTime++;
						}
					}

					delete bz;
				}
				else
				{
					cout<<"consnode_id : " << cons->cons_node_id << " satisfied !!!"<<endl;
				}
			}
				resultSolution->GetRealServers();
			}
            //开始踢直连服务器
			if(solution->cost > resultSolution->CalculateCost() || kickOutTime > 0)//kick out
			{
				if(solution->cost > resultSolution->CalculateCost())
				{
					cout<<"========  kick out worked ========="<<endl;
					solution->SolutionCopy(resultSolution);
					solution->CalculateCost();
					bigRoundTime++;  //标记大循环次数？问题：
				}
				//剔除最差服务器
				resultSolution->KickOutWorstServer(resultSolution->GetWorstServerId());
				kickOutTime--;
				StartKickOut = true;
			}
		}
			cout<<" cost : "<<solution->cost<<endl;
			time_t currentTime = time(0);
			if(difftime(currentTime, StartTime) < 80)
			{
				resultSolution->GetRealNiceServers();
			}
	}
	solution->CalculateCost();
	cout<<"cost: "<<solution->cost<<endl;
	solution->FindServerIdNotInCandidate();

	return true;

}


list<int> RandomGenerateServerIds()
{
	for(int i = 0 ; i < node_num; i++)
	{
		ALL_Nodes[i].type = 1;
	}
	list<int> result;
	srand((unsigned int)time(0));

	while(result.size() != consume_node_num)
	{
		int number = rand() % node_num ;
		if(!Contains(result, number))
		{
			result.push_back(number);
			ALL_Nodes[number].type = 2;
		}
	}
	return result;
}

void FreeSolutionSpace(Solution * _solution)
{
	for(int i = 0; i < consume_node_num; i++)
	{
		ConsNode_bw_solution * cons = _solution->bwSolution + i;
		if(cons->Initialized)
		{
			delete [] cons->paths;
			delete [] cons->bws;
		}
	}
	delete [] _solution->bwSolution;
	delete  _solution;
}

//单次暴力搜索形成解决方案
void ForceSearchGenerateSolution(list<int> ids)
{
	Solution* resultSolution = new Solution();
	resultSolution->serverIds = ids;
	resultSolution->bwSolution =new ConsNode_bw_solution[consume_node_num];
	ReInitAllBandWidths();

	while(!resultSolution->IsAllNodeSatisfied())
	{
			for(int i = 0; i < consume_node_num; i++)
			{
				ConsNode_bw_solution* cons = resultSolution->bwSolution + i ;
				cons->cons_node_id = i;

				if(!cons->isSatisfied)
				{
					cout << cons->cons_node_id << " Not Satisfied!!!"<<endl;
					ConsumeNode* cn = &ALL_ConsumeNodes[i];

					if(cons->Initialized)
					{
							delete [] cons->paths;
							delete [] cons->bws;
					}

					if(!Contains(Bad_BFS_Search_Nodes, cons->cons_node_id))
					{
						BFSzy* bz = new BFSzy(cn->net_node_id, cn->bwNeed);
						//cout<<"Begin Bfssearch id: " << cn->net_node_id<< "bwNeed: "<< cn->bwNeed<< endl;
						bfsNum = 0;
						double StartClock = clock();
						(*bz) = BFSsearch(*bz);
						double StopClock = clock();
						double searchTime =((double) (StopClock - StartClock))/ CLOCKS_PER_SEC;
						if(searchTime > 0.1)
						{
							Bad_BFS_Search_Nodes.push_back(cons->cons_node_id);
						}

						if(bz->cost != 65534 && bz->cost != 0 && bz->cost < server_deploy_cost)
						{//find solution in bfs
							cout<<"consNodeId "<< i <<" find plan! " <<endl;
							int list_length = bz->mulPath.size();
							cons->paths = new list<int>[list_length];
							cons->bws = new int[bz->bandWidth.size()];
							cons->path_num = list_length;
							cons->isSatisfied = true;
							cons->Initialized = true;

							for(int _i = 0; _i < bz->mulPath.size(); _i++)
							{
								cons->bws[_i] = bz->bandWidth[_i];
								for(int j = 0 ; j < bz->mulPath[_i].size(); j++)
								{
									cons->paths[_i].push_back(bz->mulPath[_i][j]);
								}
							}
							//检查解决方案有效性
							if(!cons->TakeBandWidths())//solution not effective
							{
								cons->isSatisfied = false;
							}
						}
						delete bz;
					}

					if(!cons->isSatisfied)//still unsatisfied deploy
					{
						cout<<"consNodeId "<< i <<" deploy server in " << cn->net_node_id << endl;
						if(!Contains(resultSolution->serverIds, cn->net_node_id))
						{
							crossNodeIds[joint_num++] = cn->net_node_id;
							ALL_Nodes[cn->net_node_id].type = 2;
							resultSolution->serverIds.push_back(cn->net_node_id);
						}

						cons->path_num = 1;
						cons->paths = new list<int>[1];
						cons->paths[0].push_back(cn->net_node_id);
						cons->bws = new int[1];
						cons->bws[0] = ALL_ConsumeNodes[cons->cons_node_id].bwNeed;
						cons->cost = 0;
						cons->isSatisfied = true;
						cons->Initialized = true;

					}
				}
				else
				{
					cout << cons->cons_node_id << " satisfied !!!"<<endl;
				}
			}
			resultSolution->GetRealServers();
		}

		resultSolution->CalculateCost();
		if(resultSolution->cost < solution->cost)
		{
			FreeSolutionSpace(solution);
			solution = resultSolution;
		}
		else
		{
			FreeSolutionSpace(resultSolution);
		}

}

//暴力搜索
string ForceSearchMainLogic()
{
	
	time(&StartTime);
	//搜索次数
	int searchTime = 0;

	solution = new Solution();
	solution->bwSolution = new ConsNode_bw_solution[consume_node_num];
	solution->cost = 999999;

	time(&StopTime);
	//当暴力搜索时间小于80秒的时候，循环
	while(difftime(StopTime,StartTime) < 80)
	{
		searchTime ++;
		ForceSearchGenerateSolution(RandomGenerateServerIds());
		time(&StopTime);
	}

	string result=OutputResult();
	bool check = solution->CheckIfSolutionEffective();


	time(&StopTime);
	cout<<solution->ToString()<<endl;
	cout<<endl << "is solution satisfied :" << check<<endl;
	cout<<"solution cost:" << solution->cost<<endl;
	cout<<"BAD BFS NODES: ";
	for(list<int>::iterator i = Bad_BFS_Search_Nodes.begin(); i != Bad_BFS_Search_Nodes.end(); i++)
	{
		cout<<" "<<*i;
	}
	cout<<endl;
	cout<<"use time : "<<difftime(StopTime ,StartTime)<< endl;
	cout<<" search for :" << searchTime << " times"<<endl;
	if(!check)
	{
		check = solution->CheckIfSolutionEffective();
		cout<<endl << "is solution satisfied :" << check<<endl;
	}

	return result;
}

string AntSearchMainLogic()
{
		time(&StartTime);

		cout<<"Enter AnstRunning"<<endl;
		AntsRunning();
		cout<<"AntsRunning finished"<<endl;

		if(InitialServerSolution())
		{
			string result=OutputResult();
			bool check = solution->CheckIfSolutionEffective();

			time(&StopTime);
			cout<<solution->ToString()<<endl;
			PrintAllNodesLevel();
			PrintAllServerLevel();
			cout<<endl << "is solution satisfied :" << check<<endl;
			cout<<"solution cost:" << solution->cost<<endl;
			cout<<"BFS SAVED TIMES: "<< BFSSavedTimes<<endl;
			cout<<"big round effected times: "<< bigRoundTime<<endl;
			cout<<"use time : "<<difftime(StopTime ,StartTime)<< endl;
			if(!check)
			{
				check = solution->CheckIfSolutionEffective();
				cout<<endl << "is solution satisfied :" << check<<endl;
			}
			return result;
		}
		else
		{
			char* result = "fail in find solution";
			cout <<result<<endl;
			return result;
		}
}



string MainLogic(char* filecontent[], int numOflines)
{
	time(&StartTime);
	Initialization(filecontent, numOflines);

	if(node_num < 10)//lowcase
	{
		return ForceSearchMainLogic();
	}
	else//middle, high case
	{
		return AntSearchMainLogic();
	}
}


#endif /* BASEDATAS_H_ */
