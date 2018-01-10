#include "deploy.h"
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include "BaseDatas.h"

//You need to complete the function 
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

	string topo_file = MainLogic(topo, line_num);
    ofstream outToFile;
    const char* newFileName=filename;
    outToFile.open(newFileName,ios::out);
    outToFile<<topo_file;
    outToFile.close();
	//write_result(topo_file, filename);

}
