/*
 * ConsNode_bw_Solution.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include "ALLSPACE_VARIABLES.h"


ConsNode_bw_solution::ConsNode_bw_solution()
{
		cons_node_id = 0;
		path_num = 0;
		paths = 0;
		bws = 0;
		cost = 0;
		isSatisfied =  false;
		Initialized = false;
}


	int ConsNode_bw_solution::CostCalculate()
	{
		int result = 0;
		for(int i = 0 ; i < path_num ; i++)
		{
			list<int>::iterator it=paths[i].begin();
			while(1)
			{
				int j1 = *it;
				it++;
				int j2 = *it;
				if(it == paths[i].end())
				{
					break;
				}
				if(j1 >= node_num || j2 >= node_num)
				{
					cout<<"Exception!!!j1 "<<j1 << " j2 "<<j2<<endl;
				}
				result += Edge_Matrix[j1][j2]->fee * bws[i];
			}
		}
		cost = result;
		return result;
	}

	string ConsNode_bw_solution::ToString()
		{
			ostringstream oStr("ConsNode_bw_solution: ");
			oStr<<"---------------------------------------------------"<<endl;
		    oStr << "cons_node_id:" << cons_node_id << " , " ;
		    oStr << "net_node_id " << ALL_ConsumeNodes[cons_node_id].net_node_id << " , ";
		    oStr << "path_num:" << path_num << " , ";
		    oStr << "cost:" << cost ;
		    oStr << " isSatisfied :" << isSatisfied;
			oStr<<endl;

		    oStr<<"paths: "<<endl;
		    for(int i=0;i<path_num;i++)
		    {
		    	oStr<<" bw "<<bws[i]<<" , ";
		    	oStr<<"path  " << i << " : ";
		    	for(list<int>::iterator it=paths[i].begin();it!=paths[i].end();it++)
		    	{
		    		oStr<<*it<< "  ";
		    	}
		    	oStr<<endl;
		    }
			return oStr.str();
		}

	//检验程序，判断解决方案中的流量是否满足要求
	bool ConsNode_bw_solution::TakeBandWidths()
	{
		int back_p = 0;
		list<int>::iterator back_j;
		bool IsBack = false;
        //检测流量是否满足要求
		for(int p = 0; p < path_num; p++)
		{
			list<int>& path = paths[p];
			list<int>::iterator i2 = path.begin();
			i2++;
			for(list<int>::iterator i = path.begin(); i2 != path.end() ; i++, i2++)
			{
				if(*i == *i2)
				{
					cout<<"Exception in take bandWidth Equal!!!"<<endl;
				}
				if(!Edge_Matrix[*i][*i2 ]->RequireForBW(*i, *i2 , bws[p]))
				{
					back_p = p;
					back_j = i;
					IsBack = true;
					break;
				}
			}
			if(IsBack)
			{
				 break;
			}
		}
//如果流量不满足要求，则流量需要进行回退
		if(!IsBack)
		{
			return true;
		}
		else
		{
			bool finished = false;
			for(int p = 0; p < path_num; p++)
			{
				list<int>& path = paths[p];
				list<int>::iterator i2 = path.begin();
				i2++;
				for(list<int>::iterator i = path.begin(); i2 != path.end() ; i++, i2++)
				{
					if(p == back_p && *back_j == *i)
					{
						finished = true;
						break;
					}
					Edge_Matrix[*i][*i2 ]->GiveBackBW(*i, *i2 , bws[p]);
				}
				if(finished)
				{
					break;
				}
			}
			return false;
		}

	}
//回退流量
	void ConsNode_bw_solution::GiveBackBandWidths()
	{
		for(int p = 0; p < path_num; p++)
		{
			list<int>& path = paths[p];
			list<int>::iterator i2 = path.begin();
			i2++;
			for(list<int>::iterator i = path.begin(); i2 != path.end() ; i++, i2++)
			{
				Edge_Matrix[*i][*i2 ]->GiveBackBW(*i, *i2 , bws[p]);
			}
		}
	}
