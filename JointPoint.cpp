/*
 * JointPoint.cpp
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include "ALLSPACE_VARIABLES.h"
JointPoint::JointPoint(int id)
    {
    	nodeId = id;
    }

    void JointPoint::OnAntPassed(Ant* ant)
    {
    	passedAnts.push_back(ant);
    }

    string JointPoint::ToString()
    {
    		ostringstream oStr("neighbors: ");
    		list<Ant*>::iterator it = passedAnts.begin();
    		oStr<<endl<<"nodeId : " << nodeId<<" ,ants_passedby: " <<passedAnts.size() <<endl << "path: ";
    		for(list<Ant*>::iterator it = passedAnts.begin();
    				it != passedAnts.end();
    				it++)
    		{
    			oStr << (**it).ToString();
    		}
    		oStr<<endl;
    		return oStr.str();
    }


