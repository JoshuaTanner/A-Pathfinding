/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Pathfind.h
Description	:	Pathfind header file.
Author   	:	Joshua Tanner
mail		:	Josh.Tanner@mediadesign.school.nz
*/

#pragma once
#include <vector>
#include <list>
#define MAX_HEIGHT 7
#define MAX_WIDTH 10


//enum of different nodes
enum NodeTypes{WALKABLE, UNWALKABLE, GOAL, START};

//Node struct which make up the search space
struct Node {
	Node() { _iGCost = 0; };
	Node(int X, int Y, Node* parent, int G, NodeTypes type) //Overloaded constructor
		:_iX(X), _iY(Y), parentNode(parent), _Type(type) { //for ease of use
		_iFCost = 0; 
		_iHCost = 0;
		if (parent != NULL)
		{
			_iGCost = G + parent->_iGCost;
		}
		else
		{
			_iGCost = 0;
		}
	}
	Node* parentNode; //parent node pointer for finding path back
	NodeTypes _Type;
	int _iFCost;
	int _iGCost;
	int _iHCost; 
	int _iX;
	int _iY;
};

class CPathfind
{

private:	
	void ProcessAdjNodes(Node* node);
	int CalculateH(Node* start, int EndX, int EndY);
	Node* LowestF();

protected:

public:
	CPathfind();
	~CPathfind();

	Node* startSearch(Node* startNode);

	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	std::vector<Node*> pathList;

	Node* SearchSpace[MAX_HEIGHT][MAX_WIDTH];

	void ResetSearchSpace();

};

