//
//  minCutsGraphs.cpp
//  MinimumCuts_Graphs
//
//  Created by Utkarsh Khandelwal on 22/02/20.
//  Copyright © 2020 Utk_Sort. All rights reserved.
//

#include "minCutsGraphs.hpp"
#include <cstdlib>
#include<assert.h>

using namespace std;

Graph::Graph()
{
    m_nodesCount = 0;
    m_edgesCount = 0;
}

int Graph::GraphInterface(const char * inputPath)
{
    ifstream input(inputPath);
    if(input.is_open())
    {
        string currString;
        std::vector<shared_ptr<Edge>> graphEdges;
        while(std::getline(input, currString))
        {
            stringstream currStrStream(currString);
            CreateNodeAndEdges(currStrStream);
        }
    }
    return 0;
}

void Graph::CreateNodeAndEdges(stringstream &stream)
{
    string currData;
    int iterCount = 0;
    std::shared_ptr<Node> mainNode;
    while(stream >> currData)
    {
        integer currDataValue = std::stoul(currData);
        std::shared_ptr<Node> currDataNode;
        bool currNodeExists = false;
        auto it = m_mapValueToNode.find(currDataValue);
        if(it == m_mapValueToNode.cend())
        {
            currDataNode = CreateGraphNode(currDataValue);
            m_mapValueToNode.insert(std::pair<int, shared_ptr<Node>>(currDataValue, currDataNode));
            currNodeExists = false;
        }
        else
        {
            currDataNode = m_mapValueToNode[currDataValue];
            currNodeExists = true;
        }
        if(iterCount == 0)
        {
            mainNode = currDataNode;
        }
        else
        {
            std::shared_ptr<Edge> edgeCommon = Graph::CreateEdge(mainNode, currDataNode, true, currNodeExists);
            if(edgeCommon == nullptr)
            {
                assert("No edge found");
            }
            m_graphUniqueEdges.insert(edgeCommon);
            mainNode->AddEdge(edgeCommon);
            currDataNode->AddEdge(edgeCommon);
        }
        iterCount++;
    }
}

shared_ptr<Node> Graph::CreateGraphNode(integer dataValue)
{
    shared_ptr<Node> currDataNode = std::make_shared<Node>(dataValue);
    AddNode(currDataNode);
    return currDataNode;
}

std::shared_ptr<Edge> Graph::CreateEdge(shared_ptr<Node> node1, shared_ptr<Node> node2, bool node1Exists, bool node2Exists)
{
    auto node1Id = node1->GetId();
    auto node2Id = node2->GetId();
    if((node1Exists && !node2Exists) || (!node1Exists && node2Exists))
    {
        auto newEdge = std::make_shared<Edge>(node1, node2);
        AddEdge(newEdge);
        return newEdge;
    }
    else
    {
        shared_ptr<Node> existingNode;
        if(node1Exists)
            existingNode = node1;
        else
            existingNode  = node2;
        
        std::set<shared_ptr<Edge>> currNodeEdges = existingNode->GetRefToEdges();
        for(auto currEdge : currNodeEdges)
        {
            auto currEdgeNodes = currEdge->GetEndNodes();
            auto id1 = currEdgeNodes[0]->GetId();
            auto id2 = currEdgeNodes[1]->GetId();
            
            bool node1Present = (id1 == node1Id) || (id1 == node2Id);
            bool node2Present = (id2 == node1Id) || (id2 == node2Id);
            if(node1Present && node2Present)
            {
                return currEdge;
            }
        }
        //Means that both of the nodes exists and edges between them are not formed yet, so we will simply create and edge and return
        auto newEdge = std::make_shared<Edge>(node1, node2);
        AddEdge(newEdge);
        return newEdge;
    }
    assert("Edge nither created nor found");
    return std::shared_ptr<Edge>();
}

integer Graph::FindCuts()
{
    while (m_nodesCount > 2)
    {
        auto edgeToCollpase = FindEdgeToCollapse();
        CollapseEdge(edgeToCollpase);
    }
    return m_edgesCount;
}

integer Graph::FindMinimumCuts(int algoRunCount)
{
    //srand(24397);
    //std::vector<integer> edgesRemaining;
    //for(int iCount = 0; iCount < algoRunCount; ++iCount)
    //{
        integer edgesReamin = FindCuts();
        //edgesRemaining.push_back(edgesReamin);
    //}
    //std::sort(edgesRemaining.begin(), edgesRemaining.end());
    //return edgesRemaining[0];
    return edgesReamin;
}

std::shared_ptr<Edge> Graph::FindEdgeToCollapse()
{
    integer minNumber = 0;
    integer maxNumber = m_edgesCount;
    //integer new_number = (rand() % (maxNumber + 1 - minNumber)) + minNumber;
    integer new_number = (rand() % (maxNumber - minNumber)) + minNumber;
    auto iter = m_graphUniqueEdges.begin();
    auto iterToUse = std::next(iter, new_number);
    if(iter == m_graphUniqueEdges.cend())
    {
        assert("Out of range iterator");
        return *(std::prev(m_graphUniqueEdges.end()));
    }

    
    return *iterToUse;
}


int Graph::CollapseEdge(std::shared_ptr<Edge> collapseThisEdge)
{
    std::array<shared_ptr<Node>, 2> endNodes = collapseThisEdge->GetEndNodes();
    std::set<shared_ptr<Edge>> edgesAttachedNode1 = endNodes[0]->GetRefToEdges();
    std::set<shared_ptr<Edge>> edgesAttachedNode2 = endNodes[1]->GetRefToEdges();

    auto collapsedNode = std::make_shared<Node>();
    AddNode(collapsedNode);
    for(auto currEdge : edgesAttachedNode1)
    {
        if(collapseThisEdge == currEdge)
            continue;
        bool replaceSuccess = currEdge->ReplaceNodes(endNodes[0], collapsedNode, currEdge);
        if(!replaceSuccess)
        {
            assert("Something wrong is going on");
        }
    }
    std::vector<shared_ptr<Edge>> toDelEdges;
    for(auto currEdge : edgesAttachedNode2)
    {
        if(collapseThisEdge == currEdge)
            continue;
        
        bool replaceSuccess = currEdge->ReplaceNodes(endNodes[1], collapsedNode, currEdge);
        if(!replaceSuccess)
        {
            assert("Something wrong is going on");
        }
        auto endNodes = currEdge->GetEndNodes();
        if(endNodes[0] == endNodes[1])
        {
            toDelEdges.push_back(currEdge);
        }
    }
        
    for(int iDelete = 0 ; iDelete < toDelEdges.size(); ++iDelete)
    {
        auto edgeToDel = toDelEdges[iDelete];
        DeleteEdge(edgeToDel);
    }
    DeleteNode(endNodes[0]);
    endNodes[0] = nullptr;
    DeleteNode(endNodes[1]);
    endNodes[1] = nullptr;

    //Finally we have to delete edge to collapse this edge
    DeleteEdge(collapseThisEdge);
    collapseThisEdge = nullptr;
    return 0;
}


std::set<shared_ptr<Edge>> & Graph::GetAllEdgesRef()
    {return m_graphUniqueEdges;}

std::set<shared_ptr<Node>> & Graph::GetAllNodesRef()
    {return m_graphAllNodes;}

integer Graph::GetNodesCount()
    {return m_nodesCount;}

integer Graph::GetEdgesCount()
    {return m_edgesCount;}

void Graph::AddNode(std::shared_ptr<Node> addNode)
{
    m_graphAllNodes.insert(addNode);
    m_nodesCount++;
}
void Graph::DeleteNode(shared_ptr<Node> deleteNode)
{
    auto it = m_graphAllNodes.find(deleteNode);
    if(it != m_graphAllNodes.cend())
    {
        m_graphAllNodes.erase(it);
        m_nodesCount--;
    }
}
     
void Graph::AddEdge(std::shared_ptr<Edge> addEdge)
{
    m_graphUniqueEdges.insert(addEdge);
    m_edgesCount++;
}

void Graph::DeleteEdge(shared_ptr<Edge> deleteEdge)
{
    auto it = m_graphUniqueEdges.find(deleteEdge);
    if(it != m_graphUniqueEdges.cend())
    {
        m_graphUniqueEdges.erase(it);
        m_edgesCount--;
    }
}



// ==============================================================================================================================
Node::Node()
{
    m_value = 0;
}

Node::Node(integer nodeValue)
{
    m_value = nodeValue;
}

bool Node::AddEdge(shared_ptr<Edge> edge)
{
    m_edges.insert(edge);
    return true;
}

std::set<shared_ptr<Edge>> & Node::GetRefToEdges()
{
    return m_edges;
}

integer Node::GetId()
{
    return m_value;
}

Edge::Edge(shared_ptr<Node> node1, shared_ptr<Node> node2)
{
    m_endNodes[0] = node1;
    m_endNodes[1] = node2;
}

const std::array<shared_ptr<Node>, 2> & Edge::GetEndNodes()
{
    return m_endNodes;
}

std::shared_ptr<Node> Edge::GetOtherNode(std::shared_ptr<Node> currNode)
{
    if(m_endNodes[0] == currNode)
        return m_endNodes[1];
    else
        return m_endNodes[0];
}

bool Edge::ReplaceNodes(std::shared_ptr<Node> oldNode, std::shared_ptr<Node> newNode, shared_ptr<Edge> &edge)
{
    if(m_endNodes[0] == oldNode)
    {
        m_endNodes[0] = newNode;
        newNode->AddEdge(edge);
        return true;
    }
    else if(m_endNodes[1] == oldNode)
    {
        m_endNodes[1] = newNode;
        newNode->AddEdge(edge);
        return true;
    }
    else
    {
        
        return false;
    }
}
