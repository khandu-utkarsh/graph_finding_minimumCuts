//
//  minCutsGraphs.hpp
//  MinimumCuts_Graphs
//
//  Created by Utkarsh Khandelwal on 22/02/20.
//  Copyright © 2020 Utk_Sort. All rights reserved.
//

#ifndef minCutsGraphs_hpp
#define minCutsGraphs_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <array>
#include <map>



typedef unsigned long integer;
using namespace std;


class Edge;

class Node
{
private:
    integer m_value;
    std::set<shared_ptr<Edge>> m_edges;
    
public:
    //Constructors
    Node();
    Node(integer nodeValue);
    Node(const Node &otherNode) = delete; //Copy Constructor
    Node(const Node &&otherNode) = delete;  //Move Constructor
    
    //Assignment Operator
    Node & operator =(const Node &otherNode) = delete;
    
    //Methods
    bool AddEdge(shared_ptr<Edge> edge);
    std::set<shared_ptr<Edge>> & GetRefToEdges();
    integer GetId();
};

class Edge
{
private:
    std::array<shared_ptr<Node>, 2> m_endNodes;
    
public:
    //Constructors
    Edge(shared_ptr<Node> node1, shared_ptr<Node> node2);
    Edge(const Edge &otherEdge) = delete;
    Edge(const Edge &&otherEdge) = delete;

    //Assignment Operators
    Edge & operator =(const Edge &otherEdge) = delete;    
    //Methods
    const std::array<shared_ptr<Node>, 2> & GetEndNodes();
    std::shared_ptr<Node> GetOtherNode(std::shared_ptr<Node> currNode);
    bool ReplaceNodes(std::shared_ptr<Node> oldNode, std::shared_ptr<Node> newNode, shared_ptr<Edge> &edge);
};

class Graph
{
public:
    
    //Constructors
    Graph();
    Graph(const Graph &objGraph) = delete;
    Graph(const Graph &&objGraph) = delete;

    //Assignment Operators
    Graph & operator =(const Graph &objGraph) = delete;
    
    int GraphInterface(const char * inputPath);    

    void CreateNodeAndEdges(stringstream &stream);
    shared_ptr<Node> CreateGraphNode(integer dataValue);
    std::shared_ptr<Edge> CreateEdge(shared_ptr<Node> node1, shared_ptr<Node> node2, bool node1Exists, bool node2Exists);

    integer FindCuts();
    integer FindMinimumCuts(int algoRunCount);
    std::shared_ptr<Edge> FindEdgeToCollapse();
    
    int CollapseEdge(std::shared_ptr<Edge> collapseThisEdge);

    //Accessors / Modifiers
    std::set<shared_ptr<Edge>> & GetAllEdgesRef();
    std::set<shared_ptr<Node>> & GetAllNodesRef();
    
    integer GetNodesCount();
    integer GetEdgesCount();
    
    void AddNode(std::shared_ptr<Node> addNode);
    void DeleteNode(shared_ptr<Node> deleteNode);
    
    void AddEdge(std::shared_ptr<Edge> addEdge);
    void DeleteEdge(shared_ptr<Edge> deleteEdge);

    private:
    std::map<integer, shared_ptr<Node>> m_mapValueToNode;

    std::set<shared_ptr<Edge>> m_graphUniqueEdges;
    std::set<shared_ptr<Node>> m_graphAllNodes;

    integer m_nodesCount;
    integer m_edgesCount;
    
    
};




#endif /* minCutsGraphs_hpp */
