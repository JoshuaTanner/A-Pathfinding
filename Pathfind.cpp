#include "Pathfind.h"
#include <algorithm>

 /************************
* Constructor: constructs pathfinder, sets all nodes 
* @author: Joshua Tanner 
* @parameter: none 
* @return: 
********************/ 
CPathfind::CPathfind()
{
	//Setting up the search space
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		for (int j = 0; j < MAX_WIDTH; j++)
			SearchSpace[i][j] = new Node(j, i, NULL, 0, WALKABLE);
	}

	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		SearchSpace[i][0]->_Type = UNWALKABLE;
		SearchSpace[i][MAX_WIDTH - 1]->_Type = UNWALKABLE;
	}

	for (int i = 0; i < MAX_WIDTH; i++)
	{
		SearchSpace[0][i]->_Type = UNWALKABLE;
		SearchSpace[MAX_HEIGHT - 1][i]->_Type = UNWALKABLE;
	}
}

 /************************
* Destructor: destroys pathfinder, deletes all alocated memory
* @author: Joshua Tanner 
* @parameter: none 
* @return: 
********************/ 
CPathfind::~CPathfind()
{
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		for (int j = 0; j < MAX_WIDTH; j++) {
			delete	SearchSpace[i][j];
			SearchSpace[i][j] = 0;
		}
	}
}

 /************************
* CalculateH: calcultes heuristic cost from current node to end node
* @author: Joshua Tanner 
* @parameter: pointer to start node, X & Y ints of end point 
* @return: H Cost int
********************/ 
int CPathfind::CalculateH(Node* start, int EndX, int EndY)
{
	int HorizontalDist = abs(EndX - start->_iX);
	int VerticalDist = abs(EndY - start->_iY);
	int total = (HorizontalDist + VerticalDist) * 10;
	return(total);
}

 /************************
* ResetSearchSpace: resets pathfinder to search again
* @author: Joshua Tanner 
* @parameter: none
* @return: void
********************/ 
void  CPathfind::ResetSearchSpace()
{
	openList.clear();
	closedList.clear();
	pathList.clear();
}

 /************************
* ProcessAdjNodes: checks all adjacent nodes to the current node to find most suitable route
* @author: Joshua Tanner 
* @parameter: pointer to current node
* @return: void
********************/ 
void CPathfind::ProcessAdjNodes(Node* node)
{
	int _X = node->_iX - 1, _Y = node->_iY - 1;
	for (int i = _Y; i < _Y + 3; i++) {
		for (int j = _X; j < _X + 3; j++) {
			if (SearchSpace[i][j]->_Type == WALKABLE || SearchSpace[i][j]->_Type == GOAL) {
				//Determine if diagonal movement
				//G Cost is 14 for diagonal movement, 10 for horizontal/vertical movement
				int _GCost;
				if (i == _Y && j == _X)
				{
					if ((SearchSpace[i + 1][j]->_Type == UNWALKABLE) || (SearchSpace[i][j + 1]->_Type == UNWALKABLE))
						continue;
					_GCost = 14 + node->_iGCost;
				}
				else if (i == _Y + 2 && j == _X)
				{
					if ((SearchSpace[i - 1][j]->_Type == UNWALKABLE) || (SearchSpace[i][j + 1]->_Type == UNWALKABLE))
						continue;
					_GCost = 14 + node->_iGCost;
				}
				else if (i == _Y && j == _X + 2)
				{
					if ((SearchSpace[i][j - 1]->_Type == UNWALKABLE) || (SearchSpace[i + 1][j]->_Type == UNWALKABLE))
						continue;
					_GCost = 14 + node->_iGCost;
				}
				else if (i == _Y + 2 && j == _X + 2)
				{
					if ((SearchSpace[i][j - 1]->_Type == UNWALKABLE) || (SearchSpace[i - 1][j]->_Type == UNWALKABLE))
						continue;
					_GCost = 14 + node->_iGCost;
				}
				else
					_GCost = 10 + node->_iGCost;
				//Check if already in openList
				if (std::find(openList.begin(), openList.end(), SearchSpace[i][j]) != openList.end() || std::find(closedList.begin(), closedList.end(), SearchSpace[i][j]) != closedList.end())
				{
					//Determine if G Cost needs to be updated
					if (_GCost < SearchSpace[i][j]->_iGCost) {
						SearchSpace[i][j]->_iGCost = _GCost;
						SearchSpace[i][j]->_iFCost = _GCost + SearchSpace[i][j]->_iHCost;
						SearchSpace[i][j]->parentNode = node;
					}
				}
				else
				{
					//Adding to openList
					SearchSpace[i][j]->_iGCost = _GCost;
					SearchSpace[i][j]->_iHCost = CalculateH(SearchSpace[i][j], 4, 4);
					SearchSpace[i][j]->_iFCost = _GCost + SearchSpace[i][j]->_iHCost;
					SearchSpace[i][j]->parentNode = node;
					openList.push_back(SearchSpace[i][j]);
				}
				//Finish if the goal is found
				if (SearchSpace[i][j]->_Type == GOAL)
				{				
					break;
				}
			}
		}
	}
	//Remove the current node from the openList 
	openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());
}

 /************************
* LowestF: iterates through the openList to find node with lowest F Cost
* @author: Joshua Tanner 
* @parameter: none
* @return: pointer to node with lowest F Cost
********************/ 
Node* CPathfind::LowestF()
{
	Node* Lowest = (openList.front());
	for (unsigned int i = 0; i < openList.size(); i++)
	{
		//Pointer is updated if a lower cost node is found
		if (openList[i]->_iFCost < Lowest->_iFCost)
			Lowest = openList[i];
	}
	return(Lowest);
}

 /************************
* startSearch: recursive function which begins pathfinding
* @author: Joshua Tanner 
* @parameter: pointer to start node
* @return: pointer to node
********************/ 
Node* CPathfind::startSearch(Node* startNode)
{
	//Check all adjacent nodes of startNode
	ProcessAdjNodes(startNode);

	//Add node to closed list and remove from open list
	closedList.push_back(startNode);

	//Check if node is goal
	if (startNode->_Type == GOAL)		
		return startNode;
	
	//No path found
	else if (openList.empty())
		return startNode;

	//Search again with node of lowest F
	startSearch(LowestF());

}
