/*
 * Edge.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include "ALLSPACE_VARIABLES.h"

Edge::Edge()
	{
		startNodeId = 0;
		endNodeId = 0;
		_bandWidth = 0;
		fee = 0;
		Upleft = 0;
		Downleft = 0;
		m_Upleft = 0;
		m_Downleft = 0;
	}

	Edge::Edge(int start, int end, int bandW, int fe)
	{
		startNodeId = start;
		endNodeId = end;
		_bandWidth = bandW;
		fee = fe;
		Upleft = bandW;
		Downleft = bandW;
		m_Upleft = 0;
		m_Downleft = 0;
	}
	string Edge::ToString()
		{
			ostringstream oStr("start: ");
		    oStr<<"start: " << startNodeId << " , " << "end: " << endNodeId;
			oStr<<" , ";
			oStr<<"bandWidth:" << _bandWidth;
			oStr<<" , fee:"<<fee << " , " << "left:" << left;
			oStr<<" Upleft: " <<Upleft <<" , " << " Downleft "<<Downleft;
			oStr<<endl;
			return oStr.str();
		}

//得到剩余流量
	int Edge::GetLeft(int start, int end)
	{
		if(start == startNodeId && end == endNodeId)
		{
			return Upleft;
		}
		else
		{
			return Downleft;
		}
	}

	bool Edge::RequireForBW(int start, int end, int bw)
	{
		if(start == startNodeId && end == endNodeId)
		{
			if(Upleft  >= bw)
			{
				//cout<<"take bandWidth in :"<< start << " and " << end<< endl;
				//cout<<"success _bandWidth "<< _bandWidth << " , left:" << Upleft <<  " need :" << bw <<endl;
//				if(start == 278 && end == 177)
//				{
//				string str;
//				cin>>str;
//				}
				Upleft -= bw;
				return true;
			}
			else
			{
				//cout<<"take bandWidth in :"<< start << " and " << end<< endl;
				//cout<<"success _bandWidth "<< _bandWidth << ", left:" <<" , left:" << Upleft <<  " need :" << bw <<endl;
				return false;
			}
		}
		else
		{
			if(Downleft >= bw)
			{
				//cout<<"take bandWidth in :"<< start << " and " << end<< endl;
				//cout<<"success _bandWidth "<< _bandWidth << " , left:" << Downleft <<  " need :" << bw<<endl;
//				if(start == 278 && end == 177)
//							{
//							string str;
//							cin>>str;
//							}
				Downleft -= bw;
				return true;
			}
			else
			{
				//cout<<"take bandWidth in :"<< start << " and " << end<< endl;
				//cout<<"fail, _bandWidth " << _bandWidth <<" , left:" << Downleft <<  " need :" << bw <<endl;
				return false;
			}
		}
	}
//流量回退
	void Edge::GiveBackBW(int start, int end, int bw)//
	{
		if(start == startNodeId && end == endNodeId)
		{
				Upleft += bw;
				if(Upleft > _bandWidth)
				{
					Upleft = _bandWidth;
				}
		}
		else
		{
			Downleft += bw;
			if(Downleft > _bandWidth)
			{
				Downleft = _bandWidth;
			}
		}
	}
//重新初始化所有流量
	void Edge::ReInitBandWidth()
	{
		Upleft = _bandWidth;
		Downleft = _bandWidth;
	}
//形成记忆保存流量
	void Edge::GenerateMemory()
	{
		m_Upleft = Upleft;
		m_Downleft = Downleft;
	}
	//恢复到最后一次记忆
	void Edge::RestoreToLastMemory()
	{
		Upleft = m_Upleft;
		Downleft = m_Downleft;
		m_Upleft = 0;
		m_Downleft = 0;
	}
