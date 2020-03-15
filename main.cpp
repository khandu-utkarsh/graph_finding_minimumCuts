//
//  main.cpp
//  MinimumCuts_Graphs
//
//  Created by Utkarsh Khandelwal on 22/02/20.
//  Copyright © 2020 Utk_Sort. All rights reserved.
//

#include "minCutsGraphs.hpp"
#include <iostream>
#include <string>


using namespace std;


integer CurrRun()
{
    string inputFileName {"/Users/utkarsh/Downloads/_f370cd8b4d3482c940e4a57f489a200b_kargerMinCut.txt"};
    //string inputFileNameTemp {"/Users/utkarsh/Downloads/tempTest.txt"};
    std::map<integer,shared_ptr<Node>> mapValueToNode;
    std::set<shared_ptr<Edge>> graphUniqueEdges;
    Graph graphObj;
    int inputRead = graphObj.GraphInterface(inputFileName.c_str());
    //int inputRead = graphObj.GraphInterface(inputFileNameTemp.c_str());
    integer minEdgesLeft = graphObj.FindMinimumCuts(10);
    return minEdgesLeft;
}

int main(int argc, const char * argv[])
{
    srand(24397);
    std::vector<integer> edgesRemaining;
    for(int iCount = 0; iCount < 100 ; ++iCount)
    {
        integer edgeLeft = CurrRun();
        edgesRemaining.push_back(edgeLeft);
    }
    std::sort(edgesRemaining.begin(), edgesRemaining.end());
    std::cout << "Minimum Count is " << edgesRemaining[0] << endl;
    return 0;
}
