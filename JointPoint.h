/*
 * JointPoint.h
 *
 *  Created on: Mar 20, 2017
 *      Author: root
 */
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "Ant.h"


#ifndef JOINTPOINT_H_
#define JOINTPOINT_H_

using namespace std;


class JointPoint{
public:
	int nodeId;
    list<Ant*>  passedAnts;

    JointPoint(int id);
    void OnAntPassed(Ant* ant);
    string ToString();
};



#endif /* JOINTPOINT_H_ */
